const http = require("http");
const fs = require("fs");
const path = require("path");
const { spawn } = require("child_process");
const { URL } = require("url");

const PORT = process.env.PORT || 3000;

const ROOT = __dirname;
const BACKEND = path.join(ROOT, "..", "backend");

// Render/Linux will use "api"
// Windows local testing can use "api.exe"
const API_EXE = process.platform === "win32"
    ? path.join(BACKEND, "api.exe")
    : path.join(BACKEND, "api");


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

        process.on("error", error => {
            reject(error);
        });

        process.on("close", code => {

            if (code !== 0) {
                reject(
                    new Error(error || "C program failed")
                );
                return;
            }

            try {
                resolve(JSON.parse(output));
            }
            catch (e) {
                reject(
                    new Error(
                        "Invalid response from C program: " + output
                    )
                );
            }

        });
    });
}


function contentType(file) {

    const ext = path.extname(file).toLowerCase();

    if (ext === ".html") {
        return "text/html";
    }

    if (ext === ".css") {
        return "text/css";
    }

    if (ext === ".js") {
        return "application/javascript";
    }

    if (ext === ".png") {
        return "image/png";
    }

    if (ext === ".jpg" || ext === ".jpeg") {
        return "image/jpeg";
    }

    if (ext === ".svg") {
        return "image/svg+xml";
    }

    if (ext === ".ico") {
        return "image/x-icon";
    }

    return "text/plain";
}


const server = http.createServer(async (req, res) => {

    const url = new URL(
        req.url,
        "http://localhost:" + PORT
    );


    // =========================
    // C BACKEND API
    // =========================

    if (url.pathname === "/api") {

        try {

            const action = url.searchParams.get("action");

            let result;


            if (action === "state") {

                result = await runC([
                    "state"
                ]);

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

            res.end(
                JSON.stringify(result)
            );

        }


        catch (error) {

            res.writeHead(500, {
                "Content-Type": "application/json"
            });

            res.end(
                JSON.stringify({
                    ok: false,
                    message: error.message
                })
            );

        }

        return;
    }


    // =========================
    // FRONTEND FILES
    // =========================

    let filePath;

    if (url.pathname === "/") {

        filePath = path.join(
            ROOT,
            "index.html"
        );

    }

    else {

        filePath = path.join(
            ROOT,
            url.pathname
        );

    }


    filePath = path.normalize(filePath);


    // Prevent access outside frontend folder

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

            "Content-Type":
                contentType(filePath)

        });


        res.end(data);

    });

});


// =========================
// START SERVER
// =========================

server.listen(
    PORT,
    "0.0.0.0",
    () => {

        console.log(
            "ParkEase running on port " + PORT
        );

        console.log(
            "Frontend -> Node.js -> C backend"
        );

    }
);