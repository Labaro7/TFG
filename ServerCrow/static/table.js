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

for (let i = 0; i < ticketProducts.length; i++) current_ticket.push(ticketProducts[i].textContent);
if (current_ticket.length > 0) lastProduct.textContent = current_ticket[added_ticket.length - 1];
lastProduct.textContent = "-";

function deleteLastOrder() {
    // Remove the li item
    ticketList.removeChild(ticketList.lastElementChild);

    // Remove from ticket array
    added_ticket.pop();
    lastProduct.textContent = added_ticket[added_ticket.length - 1];

    price.textContent -= 1; // TODO: Change por removed product price
}

function addProductToTicket(clickedProduct) {
    // Add the product to the list of products of the table
    product = { name: (clickedProduct.children)[0].textContent, price: (clickedProduct.children)[1].textContent };
    added_ticket.push(product);
    console.log(added_ticket);
    let last = added_ticket[added_ticket.length - 1];
    lastProduct.textContent = last.name + " | " + last.price;

    // Add the product in the ticket of the table
    let child = document.createElement("li");
    child.className = "addedTicketProduct";
    child.textContent = last.name + " | " + last.price;
    child.backgroundColor = "green";
    ticketList.appendChild(child);

    const price = document.getElementById("price");
    const clicked_price = (clickedProduct.children)[1].textContent;

    let clicked_price_val = parseFloat(clicked_price);
    price.textContent = price_val + clicked_price_val;
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