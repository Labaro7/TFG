// Init
let currentGridNumber = 1;
let currentButton = document.getElementById("fourthRowButton" + 1);
currentButton.style.backgroundColor = "#0a3677";
currentButton.style.color = "white";
currentButton.style.border = "1px solid black";

// First row
const n_table = document.getElementById("numTable");

// Fourth row
const li = document.getElementsByClassName("grid-product");

// Ticket
let data = [];
const ticketList = document.getElementById("ticketList");
const ticketProducts = document.getElementsByClassName("ticketProduct");
let current_ticket = [];
let added_ticket = [];
const lastProduct = document.getElementById("lastOrder");
const price = document.getElementById("price");
let price_val = parseFloat(price.textContent);

for (let i = 0; i < ticketProducts.length; i++) {
    const t = ((ticketProducts[i].children)[0].textContent).substr(1);
    const n = (ticketProducts[i].children)[1].textContent;
    const p = (ticketProducts[i].children)[2].textContent;

    current_ticket.push({ times: t, name: n, price: p });
}
console.log(current_ticket);
//if (current_ticket.length > 0) lastProduct.textContent = current_ticket[added_ticket.length - 1];
lastProduct.textContent = "-";

function deleteLastOrder() {
    let last = added_ticket[added_ticket.length - 1];
    const discount = document.getElementById("discountValue").textContent;
    price.textContent = (parseFloat(price.textContent) - (parseFloat(last.price) * (1.0 - parseFloat(discount) / 100.0))).toFixed(2); // TODO: Change por removed product price

    if (added_ticket.length > 0) {
        // Remove the li item
        ticketList.removeChild(ticketList.lastElementChild);

        added_ticket.pop();
        last = added_ticket[added_ticket.length - 1];

        // Remove from ticket array
        if (added_ticket.length > 0) lastProduct.textContent = last.name + " | " + last.price;
        else lastProduct.textContent = "-";
    }
    else {
        lastProduct.textContent = "-";
    }

}

function addProductToTicket(clickedProduct) {
    // Add the product to the list of products of the table
    product = { times: 1, name: (clickedProduct.children)[0].textContent, price: (clickedProduct.children)[1].textContent };
    added_ticket.push(product);

    let last = added_ticket[added_ticket.length - 1];
    lastProduct.textContent = last.name + " | " + last.price;

    // Add the product in the ticket of the table
    let child = document.createElement("li");
    child.className = "addedTicketProduct";
    child.textContent = last.name + " | " + last.price;
    child.backgroundColor = "green";
    ticketList.appendChild(child);

    const price = document.getElementById("price");
    const discount = document.getElementById("discountValue").textContent;

    price.textContent = (parseFloat(price.textContent) + (parseFloat(last.price) * (1.0 - parseFloat(discount) / 100.0))).toFixed(2);
}


function changeToProductsTab() {
    const ProductsTab = document.getElementById("tab-buttonProducts");
    const TicketTab = document.getElementById("tab-buttonTicket");
    ProductsTab.style.visibility = 'visible';
    TicketTab.style.visibility = 'hidden';

    const productsMenu = document.getElementById("productsMenu");
    const ticketMenu = document.getElementById("ticketMenu");
    productsMenu.style.display = 'block';
    ticketMenu.style.display = 'none';
}

function changeToTicketTab() {
    const ProductsTab = document.getElementById("tab-buttonProducts");
    const TicketTab = document.getElementById("tab-buttonTicket");
    ProductsTab.style.visibility = 'hidden';
    TicketTab.style.visibility = 'visible';

    const productsMenu = document.getElementById("productsMenu");
    const ticketMenu = document.getElementById("ticketMenu");
    productsMenu.style.display = 'none';
    ticketMenu.style.display = 'flex';
}

function toggleTab() {

}


function showPage(pageId) {
    const pages = document.querySelectorAll('.page');
    pages.forEach(page => {
        page.style.display = 'none';
    });

    const selectedPage = document.getElementById(`${pageId}-page`);
    if (selectedPage) {
        selectedPage.style.display = 'block';
    }
}

function selectButton(number) {
    let button = document.getElementById("fourthRowButton" + number);
    currentButton.style.backgroundColor = "white";
    currentButton.style.color = "black";
    currentButton.style.border = "1px solid #0a3677";
    currentButton = button;

    button = document.getElementById("fourthRowButton" + number);
    button.style.backgroundColor = "#0a3677";
    button.style.color = "white";
    currentButton.style.border = "1px solid black";
}

// TODO: It cannot display 6 or more menus
function displayMenu(clickedButton) {
    const number = clickedButton.getAttribute("data-number");
    currentGrid = document.getElementById("grid"+currentGridNumber);
    
    if (currentGrid) {
        currentGrid.style.display = 'none';
        currentGridNumber = number;

        currentGrid = document.getElementById("grid" + currentGridNumber);
        button = document.getElementById("fourthRowButton" + currentGridNumber);
        currentGrid.style.display = 'flex';
        selectButton(currentGridNumber);
    }
}

function saveOrder() {
    const order = current_ticket.concat(added_ticket);
    const url = 'https://192.168.1.66:18080/order';
    console.log(JSON.stringify(order));

    fetch(url, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(order),
    })
        .then(response => response.text())
        .then(data => {
            console.log('Response:', data);
        })
        .catch(error => {
            console.error('Error:', error);
        });

    console.log(order);

    window.location.href = "https://192.168.1.66:18080/"
}