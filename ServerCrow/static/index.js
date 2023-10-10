// Function to show the specified page
function showPage(pageId) {
    // Hide all pages
    const pages = document.querySelectorAll('.page');
    pages.forEach(page => {
        page.style.display = 'none';
    });

    // Show the selected page
    const selectedPage = document.getElementById(`${pageId}-page`);
    if (selectedPage) {
        selectedPage.style.display = 'block';
    }
}

// Function to append a number to the display
function appendNumber(number) {
    const display = document.getElementById('tableInput');
    display.value += number;
}

// Function to append a dot to the display
function appendDot() {
    const display = document.getElementById('tableInput');
    display.value += '.';
}

// Function to clear the display
function clearDisplay() {
    const display = document.getElementById('tableInput');
    display.value = '';
}

// Function to send the number to the server (modify as needed)
/*function sendNumber() {
    const display = document.getElementById('display');
    const number = display.value;

    // You can modify this part to send the number to the server
    console.log('Number to send:', number);

    data = document.getElementById('display').value;
    const requestOptions = {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(data)
    };

    fetch("/", requestOptions)
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            return response.json();
        })
        .then(data => console.log('POST request successful:', data))
        .catch(error => console.error('POST request failed:', error));

    clearDisplay();

    window.location.href = 'templates/table.html';

}*/


