const http = require("http");
const fs = require("fs");
const path = require("path");
const { spawn } = require("child_process");
const { URL } = require("url");

const PORT = 3000;
const ROOT = __dirname;
const BACKEND = path.join(ROOT, "..", "backend");
const API_EXE = path.join(BACKEND, "api.exe");

function runC(args) {
    return new Promise((resolve, reject) => {
        const process = spawn(API_EXE, args, {
            cwd: BACKEND
        });

        let output = "";
        let error = "";

        process.stdout.on("data", data => {
            output += data.toString();
        });

        process.stderr.on("data", data => {
            error += data.toString();
        });

        process.on("error", reject);

        process.on("close", code => {
            if (code !== 0) {
                reject(new Error(error || "C program failed"));
                return;
            }

            try {
                resolve(JSON.parse(output));
            } catch (e) {
                reject(new Error("Invalid response from C program: " + output));
            }
        });
    });
}

function contentType(file) {
    const ext = path.extname(file).toLowerCase();

    if (ext === ".html") return "text/html";
    if (ext === ".css") return "text/css";
    if (ext === ".js") return "application/javascript";

    return "text/plain";
}

const server = http.createServer(async (req, res) => {
    const url = new URL(req.url, "http://localhost:" + PORT);

    if (url.pathname === "/api") {
        try {
            const action = url.searchParams.get("action");

            let result;

            if (action === "state") {
                result = await runC(["state"]);
            }
            else if (action === "park") {
                result = await runC([
                    "park",
                    url.searchParams.get("studentId") || "",
                    url.searchParams.get("number") || "",
                    url.searchParams.get("type") || ""
                ]);
            }
            else if (action === "remove") {
                result = await runC([
                    "remove",
                    url.searchParams.get("number") || ""
                ]);
            }
            else if (action === "search") {
                result = await runC([
                    "search",
                    url.searchParams.get("number") || ""
                ]);
            }
            else {
                result = {
                    ok: false,
                    message: "Invalid API action."
                };
            }

            res.writeHead(200, {
                "Content-Type": "application/json"
            });

            res.end(JSON.stringify(result));
        }
        catch (error) {
            res.writeHead(500, {
                "Content-Type": "application/json"
            });

            res.end(JSON.stringify({
                ok: false,
                message: error.message
            }));
        }

        return;
    }

    let filePath = url.pathname === "/"
        ? path.join(ROOT, "index.html")
        : path.join(ROOT, url.pathname);

    filePath = path.normalize(filePath);

    if (!filePath.startsWith(ROOT)) {
        res.writeHead(403);
        res.end("Forbidden");
        return;
    }

    fs.readFile(filePath, (error, data) => {
        if (error) {
            res.writeHead(404);
            res.end("File not found");
            return;
        }

        res.writeHead(200, {
            "Content-Type": contentType(filePath)
        });

        res.end(data);
    });
});

server.listen(PORT, () => {
    console.log("ParkEase running at http://localhost:" + PORT);
    console.log("Frontend -> Node bridge -> C backend");
});
