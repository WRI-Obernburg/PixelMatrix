import './style';
import { useEffect, useState } from 'preact/hooks';

export default function App() {

	const [ws, setWs] = useState(null);

	useEffect(()=>{
		console.log("hello world");
		initWS();

	},[]);

	function initWS() {
		const gateway = `ws://${window.location.hostname}/ws`;
		console.log('Trying to open a WebSocket connection…');
    	websocket = new WebSocket(gateway);
		setWs(websocket);
    	websocket.onopen = onOpen;
    	websocket.onclose = onClose;
    	websocket.onmessage = onMessage;
	}

	function onOpen(event) {
		console.log('Connection opened');
		getReadings();
	}
	
	function onClose(event) {
		console.log('Connection closed');
		setWs(null);
		setTimeout(initWebSocket, 2000);
	}
	
	// Function that receives the message from the ESP32 with the readings
	function onMessage(event) {
		console.log(event.data);
		var myObj = JSON.parse(event.data);
		var keys = Object.keys(myObj);
	
		for (var i = 0; i < keys.length; i++){
			var key = keys[i];
			console.log(myObj[key]);
		}
	}


	function command(command) {
		console.log(command);
		if(ws!=null && ws.readyState === WebSocket.OPEN) {
			ws.send(JSON.stringify({
				command: "action",
				action: command
			}))
		}
	}

	function switchApplication(event) {
		console.log(event.target.value);
		if(ws!=null && ws.readyState === WebSocket.OPEN) {
			ws.send(JSON.stringify({
				command: "action",
				action: command
			}))
		}
	}


	return (
		<div class="flex flex-col items-center justify-center w-svw">
			<div class="bg-purple-800 w-full h-16 flex items-center justify-center gap-2">
				<h1 class="text-xl font-bold text-white text-center">LED Matrix</h1>
				<h1 clss="text-center text-xl font-bold text-white">{(ws!=null && ws.readyState === WebSocket.OPEN)?"Connected":"Connecting..."}</h1>
			</div>

			<label class="font-bold mt-4" for="application">Wähle eine Anwendung</label>
			<select name="Application" id="application" onchange={switchApplication} class="mt-1 w-4/5 h-12 border border-black rounded">
				<option value="-1">Bitte warten...</option>
				<option value="0">Rainbow</option>

			</select>

			<div class="w-4/5 max-h-28 flex justify-start flex-col mt-7">
				<div class="font-bold">Status</div>
				<div class="max-h-28 overflow-auto"> </div>
			</div>

			<div class="w-full flex flex-col justify-center mt-4 gap-3 absolute left-1/2 top-1/2 transform -translate-x-1/2 -translate-y-1/2">
				<div class="flex w-full justify-center flex-row">
					<div class="font-bold w-32 h-16 bg-green-500 rounded text-center flex items-center justify-center text-3xl hover:bg-green-600" onclick={()=>command('up')}>↑</div>
				</div>
				<div class="flex w-full justify-center flex-row gap-3">
					<div class="font-bold w-32 h-16 bg-green-500 rounded text-center flex items-center justify-center text-3xl hover:bg-green-600" onclick={()=>command('left')}>←</div>
					<div class="font-bold w-32 h-16 bg-green-500 rounded text-center flex items-center justify-center text-3xl hover:bg-green-600" onclick={()=>command('middle')}>◯</div>
					<div class="font-bold w-32 h-16 bg-green-500 rounded text-center flex items-center justify-center text-3xl hover:bg-green-600" onclick={()=>command('right')}>→</div>
					
				</div>
				<div class="flex w-full justify-center flex-row">
					<div class="font-bold w-32 h-16 bg-green-500 rounded text-center flex items-center justify-center text-3xl hover:bg-green-600" onclick={()=>command('down')}>↓</div>
				</div>

				<div class="flex w-full justify-center flex-row gap-3 mt-6">
					<div class="font-bold w-32 h-16 bg-green-500 rounded text-center flex items-center justify-center text-3xl hover:bg-green-600" onclick={()=>command('a')}>A</div>
					<div class="font-bold w-32 h-16 bg-green-500 rounded text-center flex items-center justify-center text-3xl hover:bg-green-600" onclick={()=>command('b')}>B</div>
					<div class="font-bold w-32 h-16 bg-green-500 rounded text-center flex items-center justify-center text-3xl hover:bg-green-600" onclick={()=>command('c')}>C</div>

				</div>

			</div>

		</div>
	);
}
