function showMessage(id, message) { document.getElementById(id).textContent = message; }
async function getState() { const response = await fetch('/api?action=state'); return await response.json(); }
function addSlot(container, slotName, vehicles) {
    const slot = document.createElement('div');
    const vehicle = vehicles.find(function (v) { return v.slot === slotName; });
    slot.className = vehicle ? 'slot occupied-slot' : 'slot available-slot';
    slot.innerHTML = '<strong>' + slotName + '</strong><small>' + (vehicle ? vehicle.number : 'Available') + '</small>';
    container.appendChild(slot);
}
function createSlots(data) {
    const bikeSlots = document.getElementById('bikeSlots'); const carSlots = document.getElementById('carSlots');
    bikeSlots.innerHTML = ''; carSlots.innerHTML = '';
    for (let i = 1; i <= 20; i++) addSlot(bikeSlots, 'A' + String(i).padStart(2, '0'), data.vehicles);
    for (let i = 1; i <= 10; i++) addSlot(carSlots, 'B' + String(i).padStart(2, '0'), data.vehicles);
}
async function refreshVehicles() {
    try {
        const data = await getState(); const list = document.getElementById('vehicleList'); list.innerHTML = '';
        if (data.vehicles.length === 0) list.innerHTML = '<tr><td colspan="4" class="empty">No vehicles are currently parked.</td></tr>';
        else data.vehicles.forEach(function (vehicle) { const row = document.createElement('tr'); row.innerHTML = '<td><strong>' + vehicle.slot + '</strong></td><td>' + vehicle.number + '</td><td>' + vehicle.type + '</td><td>' + vehicle.studentId + '</td>'; list.appendChild(row); });
        createSlots(data);
    } catch (error) { document.getElementById('vehicleList').innerHTML = '<tr><td colspan="4" class="empty">Could not connect to C backend.</td></tr>'; }
}
async function parkVehicle() {
    const id = document.getElementById('studentId').value, number = document.getElementById('vehicleNumber').value.trim(), type = document.getElementById('vehicleType').value;
    if (id === '' || number === '') { showMessage('parkMessage', 'Please fill all fields.'); return; }
    const response = await fetch('/api?action=park&studentId=' + encodeURIComponent(id) + '&number=' + encodeURIComponent(number) + '&type=' + encodeURIComponent(type));
    const data = await response.json(); showMessage('parkMessage', data.ok ? data.message + ' Slot: ' + data.slot : data.message);
    if (data.ok) { document.getElementById('studentId').value = ''; document.getElementById('vehicleNumber').value = ''; refreshVehicles(); }
}
async function removeVehicle() {
    const number = document.getElementById('removeNumber').value.trim(); if (number === '') { showMessage('removeMessage', 'Enter vehicle number.'); return; }
    const response = await fetch('/api?action=remove&number=' + encodeURIComponent(number)); const data = await response.json(); showMessage('removeMessage', data.message);
    if (data.ok) { document.getElementById('removeNumber').value = ''; refreshVehicles(); }
}
async function searchVehicle() {
    const number = document.getElementById('searchNumber').value.trim(); if (number === '') { document.getElementById('searchResult').textContent = 'Enter vehicle number.'; return; }
    const response = await fetch('/api?action=search&number=' + encodeURIComponent(number)); const data = await response.json();
    if (!data.found) { document.getElementById('searchResult').textContent = 'Vehicle not found.'; return; }
    document.getElementById('searchResult').innerHTML = '<strong>Vehicle Found</strong><br>Student ID: ' + data.studentId + '<br>Vehicle Number: ' + data.number + '<br>Vehicle Type: ' + data.type + '<br>Slot: ' + data.slot;
}
refreshVehicles(); setInterval(refreshVehicles, 3000);
