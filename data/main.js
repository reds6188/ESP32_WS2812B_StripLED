//var gateway = `ws://${window.location.hostname}/ws`;
//var websocket;

const HOST_NAME = "";

window.addEventListener('load', onLoad);

const light_list = ['christmas', 'rainbow', 'water', 'test'];

// HTTP GET request -------------------------------------------------
async function httpGet(url = "") {
	try {
		const response = await fetch(HOST_NAME + url, {
			method: 'GET',
		});
		if (!response.ok) {
			throw new Error(`GET Request error: ${response.status}`);
		}
		return await response.json();
	} catch (error) {
		throw new Error(error)
	}
}

// HTTP POST request ------------------------------------------------
async function httpPost(url = "", data = {}) {
	try {
		const response = await fetch(HOST_NAME + url, {
			method: 'POST',
			headers: {
				'Content-Type': 'application/json',
			},
			body: JSON.stringify(data)
		});
		if (!response.ok) {
			throw new Error(`POST Request error: ${response.status}`);
		}
		return await response.json();
	} catch (error) {
		throw new Error(error)
	}
}

function resetButtons() {
    //for(var element of document.getElementsByClassName('button')) {
    for(var element of document.querySelectorAll('#controls .button')) {
        if(element.classList.contains("grn")) {
            element.classList.replace("grn","blu");
        }
        else if(element.classList.contains("gry")) {
            element.classList.replace("gry","blu");
        }
    }
}

/*
function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
}

function onOpen(event) {
    console.log('Connection opened');

    websocket.send(JSON.stringify({
        action: 'get-num-led'
    }));
}

function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}


function onMessage(event) {
    var element;
   console.log('Data received: ' + event.data);
    let obj = JSON.parse(event.data);
    const command = obj.action;
    const value = obj.value;

    console.log(`Command is '${command}' with value '${value}'`);

    switch(command) {
        case 'get-num-led':
        case 'set-um-led':
            document.getElementById('num-led').value = value;
            resetButtons();
            break;
        case 'off':
            resetButtons();
            break;
        default:
            console.log('Received command:', command);
            resetButtons();
            element = document.getElementById(command);
            element.classList.replace("blu","grn");
            element.classList.replace("gry","grn");
            break;
    }
}
*/

function onLoad(event) {
    //initWebSocket();
    initControls();

	httpGet('/status').then(data => {
		if(data !== undefined) {
			const found = light_list.find(x => x === data.status)
			for(item of light_list) {
				const el = document.getElementById(item);
				el.classList.replace("gry", (item === found ? "grn" : "blu"));
			}
			/*
			if(found !== undefined) {
				console.log(found);
				const el = document.getElementById(found);
				el.classList.replace("blu","grn");
				el.classList.replace("gry","grn");
			}
			*/
			document.getElementById('num_led') = data.num;
			/*
			else {
				resetButtons();
			}
			*/
		}
	}).catch(err => {
		console.error(err);
	});
}

function initControls() {
    document.getElementById('grid').addEventListener('click', (e) => showSection(e));
    document.getElementById('gear').addEventListener('click', (e) => showSection(e));
    document.getElementById('save').addEventListener('click', setParams);
    document.getElementById('cancel').addEventListener('click', getParams);
    document.getElementById('christmas').addEventListener('click', sendOnOff);
    document.getElementById('rainbow').addEventListener('click', sendOnOff);
    document.getElementById('water').addEventListener('click', sendOnOff);
	document.getElementById('test').addEventListener('click', sendOnOff);
	document.getElementById('set-test').addEventListener('click', setTest);
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
		/*
        websocket.send(JSON.stringify({
            action: 'off'
        }));
		*/
    }
    else if(this.classList.contains("blu")) {
        this.classList.replace("blu","gry");
		/*
        websocket.send(JSON.stringify({
            action: this.id
        }));
		*/
    }
	httpGet('/' + this.id).then(data => {
		if(data !== undefined) {
			/*
			const found = light_list.find(x => x === data.status)
			if(found !== undefined) {
				console.log(found);
				const el = document.getElementById(found);
				el.classList.replace("blu","grn");
				el.classList.replace("gry","grn");
			}
			else {
				resetButtons();
			}
			*/

			const found = light_list.find(x => x === data.status)
			for(item of light_list) {
				const el = document.getElementById(item);
				el.classList.remove("gry", "grn", "blu");
				console.log(`Item = ${item} ; Found = ${found}`);
				el.classList.add(item === found ? "grn" : "blu");
				//el.classList.replace("grn", (item === found ? "grn" : "blu"));
				//el.classList.replace("blu", (item === found ? "grn" : "blu"));
			}
		}
	}).catch(err => {
		console.error(err);
	});
}

function getParams() {
	/*
    websocket.send(JSON.stringify({
        action: 'get-num-led',
        value: 0
    }));
	*/
	httpGet('/num_led').then(data => {
		if(data !== undefined) {
			// TO DO
		}
	}).catch(err => {
		console.error(err);
	});
}

function setParams() {
	const num_led = parseInt(document.getElementById('num-led').value);
	if (isNaN(num_led))
		num_led = 5;
	httpPost('/num_led', { num: num_led }).then(data => {
		if(data !== undefined) {
			// TO DO
		}
	}).catch(err => {
		console.error(err);
	});
	/*
    const num_led = document.getElementById('num-led').value;
    if((num_led >= 1 && num_led <= 50)){
        websocket.send(JSON.stringify({
            action: 'set-num-led',
            value: num_led
        }));
    }
    else {
        document.getElementById('param-err').style.display = 'block';
    }
	*/
}

function setTest() {
	let data = {};
	console.log(document.querySelector('.red.seed').value)
	data.red = {
		seed: parseInt(document.querySelector('.red.seed').value),
		step: parseInt(document.querySelector('.red.step').value),
	};
	data.green = {
		seed: parseInt(document.querySelector('.grn.seed').value),
		step: parseInt(document.querySelector('.grn.step').value),
	};
	data.blue = {
		seed: parseInt(document.querySelector('.blu.seed').value),
		step: parseInt(document.querySelector('.blu.step').value),
	};
	/*
	data.red.seed = parseInt(document.querySelector('.red.seed').value);
	data.green.seed = parseInt(document.querySelector('.grn.seed').value);
	data.blue.seed = parseInt(document.querySelector('.blu.seed').value);
	data.red.step = parseInt(document.querySelector('.red.step').value);
	data.green.step = parseInt(document.querySelector('.grn.step').value);
	data.blue.step = parseInt(document.querySelector('.blu.step').value);
	*/
	console.log(data)

	httpPost('/set-test', data).then(data => {
		if(data !== undefined) {
			// TO DO
		}
	}).catch(err => {
		console.error(err);
	});
}