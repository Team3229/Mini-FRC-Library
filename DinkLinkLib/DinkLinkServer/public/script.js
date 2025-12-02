const WS_PORT = 8080;
const SEND_INTERVAL_MS = 80; // send rate to server

// Element refs
const modeToggle = document.getElementById("modeToggle");
const controllerUI = document.getElementById("controllerUI");
const mobileUI = document.getElementById("mobileUI");
const statusEl = document.getElementById("status");
const debugEl = document.getElementById("debug");

// State
let useGamepad = false;
let ws;
let connected = false;

// Controller: 2 sticks (axes x4), 10 buttons
let controllerButtons = new Array(10).fill(0);
let controllerAxes = [0, 0, 0, 0]; // lx, ly, rx, ry

// Mobile: 1 joystick, 12 buttons
let mobileButtons = new Array(12).fill(0);
let mobileAxes = [0, 0]; // x,y

// Gamepad polling
let gpIndex = null;
let lastGpT = 0;

// Get the name/ID of the wanted device
function getDeviceId() {
	return document.getElementById("deviceIdInput").value.trim() || "default";
}

// Setup WebSocket using the page host (works with localhost or remote host)
function connectWS() {
	const host = location.hostname || "localhost";
	ws = new WebSocket(`ws://${host}:${WS_PORT}`);

	ws.addEventListener("open", () => {
		connected = true;
		statusEl.textContent = "WebSocket: connected";
	});

	ws.addEventListener("close", () => {
		connected = false;
		statusEl.textContent = "WebSocket: disconnected — reconnecting…";
		setTimeout(connectWS, 1000);
	});

	ws.addEventListener("error", (e) => {
		connected = false;
		statusEl.textContent = "WebSocket: error";
		// error handled by close
	});

	ws.addEventListener("message", (ev) => {
		// optional: handle server messages (e.g., ack, config)
		// debugEl.textContent = `recv: ${ev.data}`;
	});
}
connectWS();

// ----- Mode toggle handling -----
modeToggle.addEventListener("change", (e) => {
	useGamepad = e.target.checked;
	controllerUI.classList.toggle("active", useGamepad);
	mobileUI.classList.toggle("active", !useGamepad);
});

// ----- Nipple.js setup for mobile joystick -----
const mobileJoystick = nipplejs.create({
	zone: document.getElementById("mobileJoystickZone"),
	mode: "static",
	position: { left: "50%", top: "50%" },
	size: 160,
	color: "#9fbff8"
});

mobileJoystick.on("move", (evt, data) => {
	if (!data || !data.vector) return;
	// nipple vector X/Y range roughly -1..1
	mobileAxes[0] = Number(data.vector.x.toFixed(2));
	mobileAxes[1] = Number((-data.vector.y).toFixed(2)); // invert Y to typical game convention
});

mobileJoystick.on("end", () => {
	mobileAxes = [0, 0];
});

// ----- Buttons wiring (mobile & controller clickable) -----
function wireButtons(containerId, arr) {
	const container = document.getElementById(containerId);
	container.querySelectorAll("button").forEach(btn => {
		const idx = Number(btn.dataset.index);
		// touch
		btn.addEventListener("touchstart", (e) => { e.preventDefault(); arr[idx] = 1; }, { passive: false });
		btn.addEventListener("touchend", (e) => { e.preventDefault(); arr[idx] = 0; }, { passive: false });
		// mouse
		btn.addEventListener("mousedown", () => arr[idx] = 1);
		window.addEventListener("mouseup", () => arr[idx] = 0);
		// simple visual feedback
		btn.addEventListener("pointerdown", () => btn.classList.add("active"));
		btn.addEventListener("pointerup", () => btn.classList.remove("active"));
		btn.addEventListener("pointerleave", () => btn.classList.remove("active"));
	});
}
wireButtons("mobileButtons", mobileButtons);
wireButtons("controllerButtons", controllerButtons);

// ----- Gamepad API polling for controller UI -----
function scanGamepads() {
	const gps = navigator.getGamepads ? navigator.getGamepads() : [];
	for (let i = 0; i < gps.length; i++) {
		const g = gps[i];
		if (!g) continue;
		// pick the first connected gamepad
		gpIndex = i;
		return g;
	}
	gpIndex = null;
	return null;
}

function pollGamepad() {
	const now = performance.now();
	// throttle poll slightly
	if (now - lastGpT < 16) return;
	lastGpT = now;

	const g = gpIndex !== null ? navigator.getGamepads()[gpIndex] : scanGamepads();
	if (!g) return;

	// map axes: typical mapping: axes[0]=lx, axes[1]=ly, axes[2]=rx, axes[3]=ry
	controllerAxes[0] = Number((g.axes[0] || 0).toFixed(2));
	controllerAxes[1] = Number((g.axes[1] || 0).toFixed(2));
	controllerAxes[2] = Number((g.axes[2] || 0).toFixed(2));
	controllerAxes[3] = Number((g.axes[3] || 0).toFixed(2));

	// map first 10 buttons (clamp to array length)
	for (let i = 0; i < controllerButtons.length && i < g.buttons.length; i++) {
		controllerButtons[i] = g.buttons[i].pressed ? 1 : 0;
	}
}

// start polling gamepad when controller mode is active
window.addEventListener("gamepadconnected", (e) => {
	gpIndex = e.gamepad.index;
	statusEl.textContent = `Gamepad connected: ${e.gamepad.id}`;
});
window.addEventListener("gamepaddisconnected", () => {
	gpIndex = null;
	statusEl.textContent = "Gamepad disconnected";
});

// ----- Sending input to server -----
// Compose a unified payload depending on active mode
function buildPayload() {
	const id = getDeviceId();
	if (useGamepad) {
		return {
			type: "input",
			id,
			mode: "controller",
			buttons: controllerButtons.slice(),
			axes: controllerAxes.slice()
		};
	} else {
		return {
			type: "input",
			id,
			mode: "mobile",
			buttons: mobileButtons.slice(),
			axes: mobileAxes.slice()
		};
	}
}

setInterval(() => {
	if (useGamepad) pollGamepad();
	const payload = buildPayload();
	if (ws && ws.readyState === WebSocket.OPEN) {
		try {
			ws.send(JSON.stringify(payload));
			statusEl.textContent = `WS: open — mode ${payload.mode}`;
		} catch (err) {
			statusEl.textContent = "WS: send error";
		}
	} else {
		statusEl.textContent = "WS: disconnected";
	}
	// Optional debug: show current values when active
	debugEl.textContent = JSON.stringify(payload, null, 2);
}, SEND_INTERVAL_MS);
