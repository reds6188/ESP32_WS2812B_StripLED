var gateway = `ws://${window.location.hostname}/ws`;
var websocket;

let temp = 15;
let duration = 15;
let whirlpool = false;

window.addEventListener('load', onLoad);

function showSection(section) {
    console.log('cool')
    const sections = document.querySelectorAll("div.section");
    for(item of sections) {
        item.classList.add("hidden");
    }
    document.getElementById(section).classList.remove("hidden");
}

function isGreen(element) {
    return element === "grn";
}

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
    else if(command === 'color') {
        element = document.getElementById('favcolor');
        element.value = '#' + (+value).toString(16).padStart(6,'0');
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
    /*
    document.getElementById('rainbow').addEventListener('click', sendOnOff);
    document.getElementById('snowflake').addEventListener('click', sendOnOff);
    document.getElementById('colorsweep').addEventListener('click', sendOnOff);
    document.getElementById('supercar').addEventListener('click', sendOnOff);
    document.getElementById('white').addEventListener('click', sendOnOff);
    */
    document.getElementById('whirlpool').addEventListener('click', () => showSection('whirl-setting'));
    document.getElementById('temperature').addEventListener('click', () => showSection('temp-setting'));
    document.getElementById('duration').addEventListener('click', () => showSection('dur-setting'));
    for(el of document.getElementsByClassName('dw')) {
        el.addEventListener('click', (e) => decrement(e));
    }
    for(el of document.getElementsByClassName('up')) {
        el.addEventListener('click', (e) => increment(e));
    }

    document.getElementById('on-whirlpool').addEventListener('click', (e) => onOffWhirlpool(e));
    document.getElementById('temp').value = temp;
    document.getElementById('duration').value = duration;
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

function sendColor(event) {
    let hexColor = event.target.value;
    let red = parseInt(hexColor.substring(1, 3),16);
    let green = parseInt(hexColor.substring(3, 5),16);
    let blue = parseInt(hexColor.substring(5, 7),16);
    let intColor = red * 65536 + green * 256 + blue;
    websocket.send("color:" + intColor);
    console.log('Sent color: ', hexColor);
}

//---------------------------------------------

function sendShift(num) {
    websocket.send("shift:" + num);
}

function decrement(event) {
    const id = event.target.parentNode.parentNode.id
    if(id === 'temp-setting') {
        temp = document.getElementById('temp').value
        if(temp > 0)
            temp--;
        document.getElementById('temp').value = temp;
    }
    else if(id === 'dur-setting') {
        duration = document.getElementById('duration').value
        if(duration > 0)
            duration--;
        document.getElementById('duration').value = duration;
    }
}

function increment(event) {
    const id = event.target.parentNode.parentNode.id
    if(id === 'temp-setting') {
        temp = document.getElementById('temp').value
        if(temp < 30)
            temp++;
        document.getElementById('temp').value = temp;
    }
    else if(id === 'dur-setting') {
        duration = document.getElementById('duration').value
        if(duration < 30)
            duration++;
        document.getElementById('duration').value = duration;
    }
}

function onOffWhirlpool(event) {
    whirlpool = !whirlpool;
    console.log(event.target)
    if(whirlpool === true) {
        event.target.classList.replace('darkblu','grn');
        event.target.innerText = 'ON';
    }
    else {
        event.target.classList.replace('grn','darkblu');
        event.target.innerText = 'OFF';
    }
    
}