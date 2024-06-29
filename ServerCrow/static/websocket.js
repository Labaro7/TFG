const URL = 'wss://192.168.1.66:18080/subscribe';
const webSocketMessage = document.getElementById("messageContent");

function displayNewMessage(message) {
    const messages = document.getElementById("messages");

    let newMessage = document.createElement("div");
    newMessage.textContent = message;

    let space = document.createElement("div");

    messages.appendChild(newMessage);
    messages.appendChild(space);
}

function subscribeToServer() {

    const socket = new WebSocket(URL);

    socket.onopen = function (event) {
        console.log('WebSocket connected');
    };

    socket.onmessage = function (event) {
        const message = event.data;
        console.log('Message from server:', message);
        displayNewMessage(message);
    };

    socket.onerror = function (error) {
        console.error('WebSocket error:', error);
    };

    socket.onclose = function (event) {
        console.log('WebSocket closed');
    };
}

subscribeToServer();