import './style';
import { useEffect, useState } from 'preact/hooks';

export default function App() {

	const [ws, setWs] = useState(null);
	const [isConnected, setIsConnected] = useState(false);
	const [status, setStatus] = useState("");
	const [apps, setApps] = useState(["Bitte warten..."]);
	const [active_id, setActive_id] = useState(0);

	const [isUpEnabled, setIsUpEnabled] = useState(true);
	const [isLeftEnabled, setIsLeftEnabled] = useState(true);
	const [isMiddleEnabled, setIsMiddleEnabled] = useState(true);
	const [isRightEnabled, setIsRightEnabled] = useState(true);
	const [isDownEnabled, setIsDownEnabled] = useState(true);
	const [isAEnabled, setIsAEnabled] = useState(true);
	const [isBEnabled, setIsBEnabled] = useState(true);
	const [isCEnabled, setIsCEnabled] = useState(true);

	useEffect(()=>{
		document.addEventListener('keydown', (e)=>checkKey(e,ws));

		return ()=>{
			document.removeEventListener('keydown', (e)=>checkKey(e,ws));
		}

	},[ws]);

	useEffect(()=>{
		initWS();

	},[]);

	function checkKey(e,ws) {
	
		if (e.keyCode == '38') {
			// up arrow
			command('up',ws)
			e.preventDefault();
			e.stopPropagation();
		}
		else if (e.keyCode == '40') {
			// down arrow
			command('down',ws)
			e.preventDefault();
			e.stopPropagation();
		}
		else if (e.keyCode == '37') {
		   // left arrow
		   command('left',ws)
		   e.preventDefault();
			e.stopPropagation();
		}
		else if (e.keyCode == '39') {
		   // right arrow
		   command('right',ws)
		   e.preventDefault();
			e.stopPropagation();
		}
		else if (e.keyCode == '32') {
			command('middle',ws)
			e.preventDefault();
			e.stopPropagation();
		}

	
	}

	function initWS() {
		//const gateway = `ws://${window.location.hostname}/ws`;
		const gateway = `ws://matrix.local/ws`;

		console.log('Trying to open a WebSocket connection…');
    	let websocket = new WebSocket(gateway);
		setWs(websocket);
    	websocket.onopen = onOpen;
    	websocket.onclose = onClose;
    	websocket.onmessage = onMessage;
	}

	function onOpen(event) {
		console.log('Connection opened');
		setIsConnected(true);
	}
	
	function onClose(event) {
		console.log('Connection closed');
		setWs(null);
		setIsConnected(false);
		setTimeout(initWS, 2000);
	}
	
	// Function that receives the message from the ESP32 with the readings
	function onMessage(event) {
		setIsConnected(true);

		let serverpkg = JSON.parse(event.data);
		const new_status = serverpkg["status"];
		const new_controls = serverpkg["controls"]
		const new_apps = serverpkg["apps"];
		const new_active_id = serverpkg["active_id"];

		setStatus(new_status);
		setApps(new_apps);
		setActive_id(new_active_id);

		setIsUpEnabled((new_controls    & 0b00000001)!=0);
		setIsLeftEnabled((new_controls   & 0b00000010)!=0);
		setIsMiddleEnabled((new_controls & 0b00000100)!=0);
		setIsRightEnabled((new_controls  & 0b00001000)!=0);
		setIsDownEnabled((new_controls   & 0b00010000)!=0);
		setIsAEnabled((new_controls      & 0b00100000)!=0);
		setIsBEnabled((new_controls      & 0b01000000)!=0);
		setIsCEnabled((new_controls      & 0b10000000)!=0);

	}


	function command(command,ws) {
		console.log(command);
		console.log(ws);

		if(ws!=null && ws.readyState === WebSocket.OPEN) {
			ws.send(JSON.stringify({
				command: "action",
				action: command
			}))
		}
	}

	function switchApplication(event) {
		if(ws!=null && ws.readyState === WebSocket.OPEN) {
			ws.send(JSON.stringify({
				command: "switch_project",
				project: event.target.value
			}))
		}
	}


	return (
		<div class="flex flex-col items-center justify-center w-svw">
			<div class="bg-purple-800 w-full h-16 flex flex-col items-center justify-center">
				<h1 class="text-xl font-bold text-white text-center">LED Matrix</h1>
				<h1 class="text-center text-l font-bold text-white">{isConnected?"Connected":"Connecting..."}</h1>
			</div>

			<label class="font-bold mt-4" for="application">Wähle eine Anwendung</label>
			<select name="Application" id="application" value={active_id} onchange={switchApplication} class="mt-1 w-4/5 h-12 border border-black rounded">
				{

					apps.map((app,i) => {
						return <option value={i}>{app}</option>
					})
				}

			</select>

			<div class="w-4/5 max-h-28 flex justify-start flex-col mt-7">
				<div class="font-bold">Status</div>
				<div class="max-h-28 overflow-auto">{status}</div>
			</div>

			<div class="w-full flex flex-col justify-center mt-4 gap-3 self-center items-center">
				<div class="flex w-full justify-center flex-row">
					<div class={"font-bold w-36 h-24 bg-green-500 rounded text-center items-center justify-center text-3xl hover:bg-green-600 "+(isUpEnabled?"flex":"hidden")} onclick={()=>command('up',ws)}>↑</div>
				</div>
				<div class="flex w-full justify-center flex-row gap-3">
					<div class={"font-bold w-36 h-24 bg-green-500 rounded text-center items-center justify-center text-3xl hover:bg-green-600 "+(isLeftEnabled?"flex":"hidden")} onclick={()=>command('left',ws)}>←</div>
					<div class={"font-bold w-36 h-24 bg-green-500 rounded text-center items-center justify-center text-3xl hover:bg-green-600 "+(isMiddleEnabled?"flex":"hidden")} onclick={()=>command('middle',ws)}>◯</div>
					<div class={"font-bold w-36 h-24 bg-green-500 rounded text-center items-center justify-center text-3xl hover:bg-green-600 "+(isRightEnabled?"flex":"hidden")} onclick={()=>command('right',ws)}>→</div>
					
				</div>
				<div class="flex w-full justify-center flex-row">
					<div class={"font-bold w-36 h-24 bg-green-500 rounded text-center items-center justify-center text-3xl hover:bg-green-600 "+(isDownEnabled?"flex":"hidden")} onclick={()=>command('down',ws)}>↓</div>
				</div>

				<div class="flex w-full justify-center flex-row gap-3 mt-6">
					<div class={"font-bold w-36 h-24 bg-green-500 rounded text-center items-center justify-center text-3xl hover:bg-green-600 "+(isAEnabled?"flex":"hidden")} onclick={()=>command('a',ws)}>A</div>
					<div class={"font-bold w-36 h-24 bg-green-500 rounded text-center items-center justify-center text-3xl hover:bg-green-600 "+(isBEnabled?"flex":"hidden")} onclick={()=>command('b',ws)}>B</div>
					<div class={"font-bold w-36 h-24 bg-green-500 rounded text-center items-center justify-center text-3xl hover:bg-green-600 "+(isCEnabled?"flex":"hidden")} onclick={()=>command('c',ws)}>C</div>

				</div>

			</div>

		</div>
	);
}
