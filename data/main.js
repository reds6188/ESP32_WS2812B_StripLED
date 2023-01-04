var gateway = `ws://${window.location.hostname}/ws`;
var websocket;

window.addEventListener('load', onLoad);

function resetButtons() {
    for(var element of document.getElementsByClassName('button')) {
        if(element.classList.contains("grn")) {
            element.classList.replace("grn","blu");
        }
        else if(element.classList.contains("gry")) {
            element.classList.replace("gry","blu");
        }
    }
}

function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
}

function onOpen(event) {
    console.log('Connection opened');
}

function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}


function onMessage(event) {
    var element;
    var rxData = event.data.split(':')
    var command = rxData[0];
    var value = rxData[1];

    if(command === 'off') {
        resetButtons();
    }
    else {
        console.log('Received command:', command);
        resetButtons();
        element = document.getElementById(command);
        element.classList.replace("blu","grn");
        element.classList.replace("gry","grn");
    }
}

function onLoad(event) {
    initWebSocket();
    initControls();
}

function initControls() {
    document.getElementById('grid').addEventListener('click', (e) => showSection(e));
    document.getElementById('gear').addEventListener('click', (e) => showSection(e));
    document.getElementById('christmas').addEventListener('click', sendOnOff);
    document.getElementById('rainbow').addEventListener('click', sendOnOff);
    document.getElementById('water').addEventListener('click', sendOnOff);
}

function showSection(event) {
	//console.log(event.target.src.split('/')[-1])
    let id = event.target.id

	const sections = document.querySelectorAll("div.section");
	for(item of sections) {
	  item.classList.add("hidden");
	}

    const icons = document.querySelectorAll(".head-icon");
	for(item of icons) {
	  item.classList.add("hidden");
	}

    if(id === 'gear') {
        document.getElementById('grid').classList.remove("hidden");
        document.getElementById('settings').classList.remove("hidden");
    }
    else {
        document.getElementById('gear').classList.remove("hidden");
        document.getElementById('controls').classList.remove("hidden");
    }
	//document.getElementById(section).classList.remove("hidden");
}

function sendOnOff() {
    if(this.classList.contains("grn")) {
        this.classList.replace("grn","gry");
        websocket.send("off");
    }
    else if(this.classList.contains("blu")) {
        this.classList.replace("blu","gry");
        websocket.send(this.id);
    }
}