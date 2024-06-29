// Init
let currentGridNumber = 1;
let currentButton = document.getElementById("productsPagesButton" + 1);

// First row
const n_clients = document.getElementById("numberOfClients");

// Third row
const multiplier_array = [1, 2, 5];
let multiplier_index = 0;

// Fourth row
const li = document.getElementsByClassName("grid-product");
let page_number = 1;

// Ticket
let data = [];
const ticketList = document.getElementById("ticketList");
const ticketProducts = document.getElementsByClassName("ticketProduct");
let current_ticket = [];
let added_ticket = [];
let modified_ticket = [];
let deleted_ticket = [];
const lastProduct = document.getElementById("lastOrder");
const price = document.getElementById("price");
let price_val = parseFloat(price.textContent);

// Hide the current products division if there are no products in the ticket
if (ticketProducts.length > 0) document.getElementById("divisionCurrentProducts").style.display = "flex";

for (let i = 0; i < ticketProducts.length; i++) {
    const t = parseInt(((ticketProducts[i].children)[0].textContent).substr(1));
    const n = (ticketProducts[i].children)[1].textContent;
    const p = parseFloat((ticketProducts[i].children)[2].textContent);
    const d = "";

    current_ticket.push({ times: t, name: n, price: p, details: d });
}

//if (current_ticket.length > 0) lastProduct.textContent = current_ticket[added_ticket.length - 1];
lastProduct.textContent = "-";

// Ticket
let modifyingProduct;

sortTicketAlphabetically();

function sortTicketAlphabetically() {
    const ul = document.getElementById('ticketList');
    const li = document.querySelectorAll('#ticketList li');
    let li_array = Array.from(li);
    li_array = li_array.slice(0, -1);

    li_array.sort((a, b) => {
        const nameA = a.querySelector('.productNames').textContent.toLowerCase();
        const nameB = b.querySelector('.productNames').textContent.toLowerCase();

        if (nameA === nameB) {
            const detailsA = a.querySelector('.details').textContent.toLowerCase();
            const detailsB = b.querySelector('.details').textContent.toLowerCase();
            return detailsA.localeCompare(detailsB);
        }

        return nameA.localeCompare(nameB);
    });

    ul.innerHTML = '';

    li_array.forEach((item) => {
        ticketList.appendChild(item);
    });

    ul.innerHTML += '<div id="divisionAddedProducts"><div class="horizontal-line"></div>ADDED<div class="horizontal-line"></div></div>'
}

function showIngredientsAndAllergens(clickedElement) {
    event.stopPropagation(); // So the child onclick is on top of the parents'

    const ingredientsAndAllergensMenu = document.getElementById("ingredientsAndAllergensMenu");
    const ticketMenu = document.getElementById("ticketMenu");
    const productsMenu = document.getElementById("productsMenu");
    const tab = document.getElementsByClassName("tab");
    const productName = document.getElementById("productName");
    const ingredientsAndAllergens = document.getElementById("ingredientsAndAllergens");
    const ingredients = ingredientsAndAllergens.querySelector(".ingredients");
    const allergens = ingredientsAndAllergens.querySelector(".allergens");
    const ingredientsList = document.getElementById("ingredientsList");
    const allergensList = document.getElementById("allergensList");

    if ((clickedElement.id === "infoButton") || (lastOrder.textContent != "-" && clickedElement.id == "ingsAllergsButton")) {
        let lastOrderName;

        if (clickedElement.id === "ingsAllergsButton") {
            const lastOrder = document.getElementById("lastOrder");
            lastOrderName = lastOrder.textContent.substring(lastOrder.textContent.indexOf(" ") + 1, lastOrder.textContent.indexOf(" ", lastOrder.textContent.lastIndexOf(" |")));
        } else {
            const selectedProductName = clickedElement.parentElement.parentElement.children[1].children[0].textContent;
            lastOrderName = selectedProductName;
        }

        const productList = document.querySelectorAll('div.products-names, li.product');

        ingredientsAndAllergensMenu.style.display = "flex";
        ticketMenu.style.pointerEvents = "none";
        ticketMenu.style.filter = "blur(10px) grayscale(100%)";
        productsMenu.style.pointerEvents = "none";
        productsMenu.style.filter = "blur(10px) grayscale(100%)";
        document.body.style.backgroundColor.filter = "grayscale(100%)";
        tab[0].style.pointerEvents = "none";
        tab[0].style.filter = "blur(10px) grayscale(100%)";
        document.body.style.backgroundColor = "#b0b0b0";

        productName.textContent = lastOrderName;
        ingredientsList.innerHTML = "";
        allergensList.innerHTML = "";

        productList.forEach(product => {
            if (product.textContent === lastOrderName) {
                const productIngredients = product.parentElement.children[2].children;
                const productAllergens = product.parentElement.children[3].children;

                if (productIngredients.length) {
                    ingredients.style.display = "flex";

                    let i = 0;

                    for (let productIngredient of productIngredients) {
                        let isEven = i % 2 === 0 ? true : false;
                        const productIngredientName = productIngredient.textContent;
                        addListElement(ingredientsList, "ingredient", productIngredientName, isEven);

                        i++;
                    }
                }
                else {
                    ingredients.style.display = "none";
                }

                if (productAllergens.length) {
                    allergens.style.display = "flex";

                    let i = 0;
                    for (let productAllergen of productAllergens) {
                        let isEven = i % 2 === 0 ? true : false;

                        const productAllergenName = productAllergen.textContent;
                        addListElement(allergensList, "allergen", productAllergenName, isEven);

                        i++;
                    }
                }
                else {
                    allergens.style.display = "none";
                }
            }
        });

        if (clickedElement.id === "infoButton") {
            productName.textContent = clickedElement.parentElement.parentElement.children[1].children[0].textContent;
        }
    } else {
        console.log("No ingredients nor allergens to display");
        ingredientsList.innerHTML = "";
        allergensList.innerHTML = "";
    }
}

function addListElement(list, className, textContent, isEven) {
    const newElement = document.createElement("li");
    newElement.className = className;
    newElement.textContent = textContent;

    if (isEven) newElement.style.backgroundColor = "rgb(237, 237, 237)";
    list.appendChild(newElement);
}

function goBackToTable() {
    let ingredientsAndAllergensMenu = document.getElementById("ingredientsAndAllergensMenu");
    let productsMenu = document.getElementById("productsMenu");
    let tab = document.getElementsByClassName("tab");
    let ticketMenu = document.getElementById("ticketMenu");

    ingredientsAndAllergensMenu.style.display = "none";
    productsMenu.style.pointerEvents = "auto";
    productsMenu.style.filter = "blur(0px) grayscale(0%)";
    productsMenu.style.filter = "";
    tab[0].style.pointerEvents = "auto";
    tab[0].style.filter = "blur(0px) grayscale(0%)";
    document.body.style.backgroundColor = "rgb(9, 43, 92)";
    ticketMenu.style.pointerEvents = "auto";
    ticketMenu.style.filter = "blur(0px) grayscale(0%)";
}

function goBack() {
    setTimeout(() => { window.location.href = "/"; }, 10);
}

function searchByName(array, name) {
    return array.find(function (obj) {
        return obj.name === name;
    });
}
function deleteLastOrder() {
    let lastOrder = secondRow.children[0];
    let lastOrderName = lastOrder.textContent.substring(lastOrder.textContent.indexOf(" ") + 1, lastOrder.textContent.indexOf(" ", lastOrder.textContent.lastIndexOf(" |")));
    let currency = " " + price.textContent[price.textContent.length - 1];

    if (added_ticket.length > 0) {
        let last = searchByName(added_ticket, lastOrderName);
        let lastIndex = added_ticket.indexOf(last);
        const discount = document.getElementById("discountValue").textContent;
        price.textContent = (parseFloat(price.textContent) - (parseFloat(last.price) * parseInt(last.times) * (1.0 - parseFloat(discount) / 100.0))).toFixed(2) + currency; // TODO: Change por removed product price

        // Remove the li item
        let modifyDeleteMenu = document.getElementById("modifyDeleteMenu");
        let addedTicketProducts = document.querySelectorAll('li.addedTicketProduct');

        for (let addedTicket of addedTicketProducts) {
            let ticketElements = addedTicket.querySelectorAll('div.productNames');

            ticketElements.forEach(function (item) {
                if (item.textContent.includes(lastOrderName)) {
                    let ticketElement = item.parentElement.parentElement;

                    if (ticketElement.contains(modifyDeleteMenu)) {
                        modifyDeleteMenu.style.display = "none";
                        document.body.appendChild(modifyDeleteMenu);
                    }

                    ticketList.removeChild(ticketElement);
                }
            });
        }

        added_ticket.splice(lastIndex, 1);
        last = added_ticket[added_ticket.length - 1];

        // Remove from ticket array
        let addedDivision = document.getElementById("divisionAddedProducts");
        let found = added_ticket.find(prod => prod.name === last.name);
        if (added_ticket.length > 0) lastProduct.textContent = "x" + found["times"] + " " + last.name + " | " + last.price;
        else lastProduct.textContent = "-", addedDivision.style.display = "none";
    }
    else {
        lastProduct.textContent = "-";
    }
}

function toggleMultiplier() {
    let current_multiplier = document.getElementById(multiplier_array[multiplier_index].toString());
    multiplier_index = (multiplier_index + 1) % multiplier_array.length;
    let next_multiplier = document.getElementById((multiplier_array[multiplier_index]).toString());

    current_multiplier.style.display = "none";
    next_multiplier.style.display = "flex";
}

function addProductToTicket(clickedProduct) {
    const secondRow = document.getElementById("secondRow");
    secondRow.classList.toggle('clicked');
    setTimeout(function () {
        secondRow.classList.remove('clicked');
    }, 125);

    const price = document.getElementById("price");
    const discount = document.getElementById("discountValue").textContent;

    let last = "x" + + added_ticket[added_ticket.length - 1];
    let found = added_ticket.find(prod => (prod.name === (clickedProduct.children)[0].textContent && prod.details === ""));

    if (added_ticket.length === 0) {
        document.getElementById("divisionAddedProducts").style.display = "flex";
    }

    let currency = " " + price.textContent[price.textContent.length - 1];
    if (found) {
        found["times"] += multiplier_array[multiplier_index];

        let foundAddedProduct = Array.from(document.getElementsByClassName("addedTicketProduct")).find(element => element.children[1].children[0].textContent === found.name && element.children[1].children[1].textContent === found.details);

        foundAddedProduct.children[0].children[0].textContent = "x" + found.times;
        foundAddedProduct.children[3].textContent = (found.times * found.price).toFixed(2);

        lastProduct.textContent = "x" + found["times"] + " " + found.name + " | " + found.price;

        price.textContent = (parseFloat(price.textContent) + (parseFloat(found.price) * multiplier_array[multiplier_index] * (1.0 - parseFloat(discount) / 100.0))).toFixed(2) + currency;
    }
    else {
        let product = {
            times: multiplier_array[multiplier_index],
            name: (clickedProduct.children)[0].textContent,
            price: parseFloat((clickedProduct.children)[1].textContent).toFixed(2),
            details: ""
        };

        added_ticket.push(product);

        last = added_ticket[added_ticket.length - 1];
        lastProduct.textContent = "x" + multiplier_array[multiplier_index] + " " + last.name + " | " + last.price;

        // Add the product in the ticket of the table
        let child = document.createElement("li");
        child.className = "addedTicketProduct";
        child.backgroundColor = "green";
        child.onclick = function () {
            openModifyDeleteMenu(this);
        };

        let child_product_times = document.createElement("div");
        child_product_times.className = "productTimes";
        child_product_times.textContent = "x" + multiplier_array[multiplier_index];

        let child_old_times = document.createElement("div");
        child_old_times.className = "oldProductTimes";

        let child_times = document.createElement("div");
        child_times.className = "times";
        child_times.appendChild(child_product_times);
        child_times.appendChild(child_old_times);
        child.appendChild(child_times);

        let child_container = document.createElement("div");
        child_container.className = "container";

        let child_name = document.createElement("div");
        child_name.className = "productNames";
        child_name.textContent = product["name"];
        child_container.appendChild(child_name);

        let child_details = document.createElement("div");
        child_details.className = "details";
        child_details.textContent = product["details"];
        child_container.appendChild(child_details);

        child.appendChild(child_container);

        let child_price = document.createElement("div");
        child_price.className = "productPrices";
        child_price.textContent = product["price"];
        child.appendChild(child_price);

        let child_total_price = document.createElement("div");
        child_total_price.className = "productTotalPrices";
        child_total_price.textContent = (product["times"] * product["price"]).toFixed(2);
        child.appendChild(child_total_price);

        ticketList.appendChild(child);

        price.textContent = (parseFloat(price.textContent) + (parseFloat(last.price) * multiplier_array[multiplier_index] * (1.0 - parseFloat(discount) / 100.0))).toFixed(2) + currency;
    }
}

function openDeployable(clickedDeployable) {
    const grid = "grid" + page_number;
    const grid_products = document.getElementById(grid).querySelectorAll("li");
    for (let i of grid_products) {
        i.style.display = "none";
    }

    const clickedDeployableProducts = document.querySelectorAll("li[data-deployable='" + clickedDeployable.dataset.id + "']");
    for (let i of clickedDeployableProducts) {
        i.style.display = "flex";
    }
}


function changeToProductsTab() {
    const productsTab = document.getElementById("productsTab");
    const products = document.getElementById("products")
    const productsIcon = document.getElementById("productsIcon");
    const ticketTab = document.getElementById("ticketTab");
    const ticket = document.getElementById("ticket");
    const ticketIcon = document.getElementById("ticketIconPath");
    const modifyDeleteMenu = document.getElementById("modifyDeleteMenu");

    productsTab.style.backgroundColor = "#bdd7fc";
    productsTab.style.borderTop = "5px solid rgb(28, 89, 176)";

    products.style.color = "black";

    productsIcon.style.fill = "black";

    ticketTab.style.backgroundColor = "rgb(9, 43, 92)";
    ticketTab.style.borderTop = "0px";
    ticketTab.style.borderTop = "5px solid rgb(9, 43, 92)";

    ticket.style.color = "white";

    ticketIcon.style.stroke = "white";

    modifyDeleteMenu.style.visibility = 'hidden';

    const productsMenu = document.getElementById("productsMenu");
    const ticketMenu = document.getElementById("ticketMenu");
    productsMenu.style.display = 'block';
    ticketMenu.style.display = 'none';
}


function changeToTicketTab() {
    const productsTab = document.getElementById("productsTab");
    const products = document.getElementById("products")
    const productsIcon = document.getElementById("productsIcon");
    const ticketTab = document.getElementById("ticketTab");
    const ticket = document.getElementById("ticket");
    const ticketIcon = document.getElementById("ticketIconPath");
    const modifyDeleteMenu = document.getElementById("modifyDeleteMenu");

    productsTab.style.backgroundColor = "rgb(9, 43, 92)";
    productsTab.style.borderTop = "0px";
    productsTab.style.borderTop = "5px solid rgb(9, 43, 92)";

    products.style.color = "white";

    productsIcon.style.fill = "white";

    ticketTab.style.backgroundColor = "#bdd7fc";
    ticketTab.style.borderTop = "5px solid rgb(28, 89, 176)";

    ticket.style.color = "black";

    ticketIcon.style.stroke = "black";

    modifyDeleteMenu.style.visibility = 'visible';

    const productsMenu = document.getElementById("productsMenu");
    const ticketMenu = document.getElementById("ticketMenu");
    productsMenu.style.display = 'none';
    ticketMenu.style.display = 'flex';
}

function openNumClientsMenu() {
    const numClientsMenu = document.getElementById("numClientsMenu");
    let tab = document.getElementsByClassName("tab");
    let productsMenu = document.getElementById("productsMenu");

    numClientsInput.value = "";
    numClientsMenu.style.display = "flex";
    tab[0].style.pointerEvents = "none";
    tab[0].style.filter = "blur(5px) grayscale(100%)";
    productsMenu.style.pointerEvents = "none";
    productsMenu.style.filter = "blur(5px) grayscale(100%)";
    document.body.style.backgroundColor = "#b0b0b0";
}

function cancelNumClientsMenu() {
    let tab = document.getElementsByClassName("tab");
    let productsMenu = document.getElementById("productsMenu");
    let numClientsMenu = document.getElementById("numClientsMenu");

    numClientsMenu.style.display = "none";
    tab[0].style.pointerEvents = "auto";
    tab[0].style.filter = "blur(0px) grayscale(0%)";
    productsMenu.style.pointerEvents = "auto";
    productsMenu.style.filter = "blur(0px) grayscale(0%)";
    document.body.style.backgroundColor = "#bdd7fc";
}

function setNumClients() {
    let tab = document.getElementsByClassName("tab");
    let productsMenu = document.getElementById("productsMenu");
    const numClientsInput = document.getElementById("numClientsInput");
    const numberOfClients = document.getElementById("numberOfClients");
    const numClientsMenu = document.getElementById("numClientsMenu");

    if (numClientsInput.value === "") numClientsInput.value = "0";

    numberOfClients.textContent = numClientsInput.value;

    numClientsMenu.style.display = "none";
    tab[0].style.pointerEvents = "auto";
    tab[0].style.filter = "blur(0px) grayscale(0%)";
    productsMenu.style.pointerEvents = "auto";
    productsMenu.style.filter = "blur(0px) grayscale(0%)";
    document.body.style.backgroundColor = "#bdd7fc";
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
    let button = document.getElementById("productsPagesButton" + number);
    currentButton.style.backgroundColor = "rgb(9, 43, 92)";
    currentButton.style.color = "white";
    currentButton.style.borderBottom = "1px solid white";
    currentButton = button;

    button = document.getElementById("productsPagesButton" + number);
    button.style.backgroundColor = "rgb(28, 89, 176)";
    button.style.color = "white";
    button.style.borderBottom = "0px";
}


// TODO: It cannot display 6 or more menus
function displayMenu(clickedButton) {
    const number = clickedButton.getAttribute("data-number");
    page_number = number;
    currentGrid = document.getElementById("grid" + currentGridNumber);

    if (currentGrid) {
        currentGrid.style.display = 'none';
        currentGridNumber = number;

        currentGrid = document.getElementById("grid" + currentGridNumber);
        button = document.getElementById("productsPagesButton" + currentGridNumber);
        currentGrid.style.display = 'flex';
        const currentGridDeployableElements = currentGrid.querySelectorAll("li");

        for (let i of currentGridDeployableElements) {
            i.style.display = "none";
        }

        const currentGridElements = currentGrid.children;
        for (let i of currentGridElements) {
            i.style.display = "flex";
        }

        const currentGridDeployables = currentGrid.querySelectorAll(".deployable-product");
        for (let i of currentGridDeployables) {
            i.style.display = "none";
        }
        selectButton(currentGridNumber);
    }
}


function saveOrder() {
    const n_table = parseInt((document.getElementById("numTable").textContent).substr(7)); // Use substr(7) to eliminate "Table: "

    if (n_clients.textContent === null || n_clients.textContent === "") n_clients.textContent = "0";

    const data = {
        order: current_ticket,
        added: added_ticket,
        modified: modified_ticket,
        deleted: deleted_ticket,
        n_table: n_table,
        n_clients: parseInt(n_clients.textContent),
        employee: getCookie("employee_name")
    };

    const url = "/order";

    fetch(url, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(data),
    })
        .then(response => response.text())
        .then(data => {
            //console.log('Response:', data);
        })
        .catch(error => {
            //console.error('Error:', error);
        });

    // If setTimeout is not used, when the index.html is received
    // it doesn't have the tables prices updated because
    // it didn't have enough time to retrieve the new information
    setTimeout(() => { window.location.href = "/"; }, 100);
}

function openMoveTableMenu() {
    let tab = document.getElementsByClassName("tab");
    let productsMenu = document.getElementById("productsMenu");
    let moveTableMenu = document.getElementById("moveTableMenu");

    tab[0].style.pointerEvents = "none";
    tab[0].style.filter = "blur(10px) grayscale(100%)";
    productsMenu.style.pointerEvents = "none";
    productsMenu.style.filter = "blur(10px) grayscale(100%)";
    moveTableMenu.style.display = "flex";
    document.body.style.backgroundColor = "#b0b0b0";
}

function appendNumber(number) {
    const display = document.getElementById('tableInput');
    const display2 = document.getElementById('amountInput');
    const display3 = document.getElementById("paidInput");
    const display4 = document.getElementById("numClientsInput");

    if (number !== ".") {
        if (display.value.length < 5) display.value += number;
        if (display2.value.length < 5) display2.value += number;
        display3.value += number;
        if (display4.value.length < 5) display4.value += number;

    }
    else {
        if (display3.value.indexOf(".") === -1 && display3.value !== "" ) {
            display3.value += ".";  
        }
    }
}

function clearDisplay() {
    const display = document.getElementById('tableInput');
    const display3 = document.getElementById("paidInput");
    const display2 = document.getElementById('amountInput');
    const display4 = document.getElementById("numClientsInput");

    display.value = "";
    display2.value = "";
    display3.value = "";
    display4.value = "";
}

function acceptMoveTableMenu() {
    const n_table = document.getElementById("numTable");
    const api_moveTable = "/moveTable";
    let _current_n_table = parseInt(n_table.textContent.substr(7));
    let _new_n_table = parseInt(document.getElementById("tableInput").value);
    let data = {
        current_n_table: _current_n_table,
        new_n_table: _new_n_table,
        employee: getCookie("employee_name")
    };

    fetch(api_moveTable, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(data),
    })
        .then(response => response.text())
        .then(data => {
            //console.log('Response:', data);
        })
        .catch(error => {
            //console.error('Error:', error);
        });

    setTimeout(() => { window.location.href = "/"; }, 100);
}

function cancelMoveTableMenu() {
    let tab = document.getElementsByClassName("tab");
    let productsMenu = document.getElementById("productsMenu");
    let moveTableMenu = document.getElementById("moveTableMenu");
    let moveTableWarningMenu = document.getElementById("moveTableWarningMenu");

    tab[0].style.pointerEvents = "auto";
    tab[0].style.filter = "blur(0px) grayscale(0%)";
    productsMenu.style.pointerEvents = "auto";
    productsMenu.style.filter = "blur(0px) grayscale(0%)";
    moveTableMenu.style.display = "none";
    moveTableWarningMenu.style.display = "none";
    document.body.style.backgroundColor = "#bdd7fc";
}

function moveTable() {
    const n_table = document.getElementById("numTable");
    const current_n_table = parseInt(n_table.textContent.substr(7));
    const new_n_table = parseInt(document.getElementById("tableInput").value);

    const api_currentTables = "/currentTables";
    const api_moveTable = "/moveTable";

    fetch(api_currentTables)
        .then(response => response.json())
        .then(data => {
            //console.log('Response:', data);
            for (let i of data["tables"]) {
                if (i === parseInt(new_n_table)) {
                    // Display moveTableWarning
                    const moveTableMenu = document.getElementById("moveTableMenu");
                    moveTableMenu.style.display = "none";

                    let tab = document.getElementsByClassName("tab");
                    let productsMenu = document.getElementById("productsMenu");
                    const warning = document.getElementById("moveTableWarningMenu");

                    tab[0].style.pointerEvents = "none";
                    tab[0].style.filter = "blur(5px) grayscale(100%)";
                    productsMenu.style.pointerEvents = "none";
                    productsMenu.style.filter = "blur(5px) grayscale(100%)";
                    warning.style.display = "flex";

                    return;
                }
            }

            data = {
                current_n_table: current_n_table,
                new_n_table: new_n_table,
                employee: getCookie("employee_name")
            };

            fetch(api_moveTable, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                    'Set-Cookie': getCookie("session_token"),
                    'Set-Cookie': getCookie("employee_name")
                },
                body: JSON.stringify(data),
            })
                .then(response => response.text())
                .then(data => {
                    console.log('Responsaal:', data);
                })
                .catch(error => {
                    console.error('Errool:', error);
                });

            setTimeout(() => { window.location.href = "/"; }, 100);
        })
        .catch(error => {
            console.error('Error:', error);
        });
}

function formatDate(date) {
    const year = date.getFullYear();
    const month = String(date.getMonth() + 1).padStart(2, '0');
    const day = String(date.getDate()).padStart(2, '0');
    const hours = String(date.getHours()).padStart(2, '0');
    const minutes = String(date.getMinutes()).padStart(2, '0');
    const seconds = String(date.getSeconds()).padStart(2, '0');

    return `${year}-${month}-${day} ${hours}:${minutes}:${seconds}`;
}

function payTable() {
    const n_table = document.getElementById("numTable");
    let billPrice = price.textContent.slice(0, -2);
    const paidPrice = document.getElementById("paidInput").value;

    if (n_clients.textContent === null || n_clients.textContent === "") n_clients.textContent = "0";

    if (paidPrice === "") paidPrice = 0.0;

    let data = {
        n_table: parseInt(n_table.textContent.substr(7)),
        n_clients: parseInt(n_clients.textContent),
        ticket: current_ticket,
        bill: parseFloat(billPrice),
        paid: parseFloat(paidPrice),
        discount: 0.0,
        method: "Card",
        date: formatDate(new Date())
    }

    console.log("DATA", data);

    //debugger;


    if (getCookie("employee_name") != null) {
        data.employee = getCookie("employee_name")
    }

    const url = "/payTable";
    fetch(url, {    
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(data),
    })
        .then(response => response.text())
        .then(data => {
            //console.log('Response:', data);
        })
        .catch(error => {
            //console.error('Error:', error);
        });

    setTimeout(() => { window.location.href = "/"; }, 100);
}

function openDeleteTableWarningMenu() {
    const deleteTableWarningMenu = document.getElementById("deleteTableWarningMenu");
    let tab = document.getElementsByClassName("tab");
    let productsMenu = document.getElementById("productsMenu");
    let ticketMenu = document.getElementById("ticketMenu");

    deleteTableWarningMenu.style.display = "flex";
    tab[0].style.pointerEvents = "none";
    tab[0].style.filter = "blur(5px) grayscale(100%)";
    ticketMenu.style.pointerEvents = "none";
    ticketMenu.style.filter = "blur(5px) grayscale(100%)";
    productsMenu.style.pointerEvents = "none";
    productsMenu.style.filter = "blur(5px) grayscale(100%)";
    document.body.style.backgroundColor = "#bdd7fc";
}

function cancelDeleteTableWarningMenu() {
    const deleteTableWarningMenu = document.getElementById("deleteTableWarningMenu");
    let tab = document.getElementsByClassName("tab");
    let ticketMenu = document.getElementById("ticketMenu");
    let productsMenu = document.getElementById("productsMenu");

    deleteTableWarningMenu.style.display = "none";
    tab[0].style.pointerEvents = "auto";
    tab[0].style.filter = "blur(0px) grayscale(0%)";
    ticketMenu.style.pointerEvents = "auto";
    ticketMenu.style.filter = "blur(0px) grayscale(0%)";
    productsMenu.style.pointerEvents = "none";
    productsMenu.style.filter = "blur(0px) grayscale(0%)";
    document.body.style.backgroundColor = "#b0b0b0";
;}

function deleteTable() {
    const n_table = document.getElementById("numTable");

    let data = {
        n_table: n_table.textContent.substr(7),
        employee: "Adri"
    }

    const url = "/deleteTable";
    fetch(url, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(data),
    })
        .then(response => response.text())
        .then(data => {
            //console.log('Response:', data);
        })
        .catch(error => {
            //console.error('Error:', error);
        });

    setTimeout(() => { window.location.href = "/"; }, 100);
}

function openPayTableMenu() {
    const payTableMenu = document.getElementById("payTableMenu");
    let tab = document.getElementsByClassName("tab");
    let ticketMenu = document.getElementById("ticketMenu");

    payTableMenu.style.display = "flex";
    tab[0].style.pointerEvents = "none";
    tab[0].style.filter = "blur(5px) grayscale(100%)";
    ticketMenu.style.pointerEvents = "none";
    ticketMenu.style.filter = "blur(5px) grayscale(100%)";
    document.body.style.backgroundColor = "#b0b0b0";
}

function cancelPayTableMenu() {
    const payTableMenu = document.getElementById("payTableMenu");
    let tab = document.getElementsByClassName("tab");
    let ticketMenu = document.getElementById("ticketMenu");

    payTableMenu.style.display = "none";
    tab[0].style.pointerEvents = "auto";
    tab[0].style.filter = "blur(0px) grayscale(0%)";
    ticketMenu.style.pointerEvents = "auto";
    ticketMenu.style.filter = "blur(0px) grayscale(0%)";
    document.body.style.backgroundColor = "#bdd7fc";
}

function openModifyDeleteMenu(clickedProduct) {
    let modifyDeleteMenu = document.getElementById("modifyDeleteMenu");
    let individualPrice = clickedProduct.children[3];

    clickedProduct.appendChild(document.getElementById("modifyDeleteMenu"));

    if (modifyingProduct == clickedProduct) { // First selected item?
        modifyingProduct = null;
        if (clickedProduct.style.backgroundColor !== "rgb(196, 31, 31)") {
            if (clickedProduct.className == "ticketProduct") {
                clickedProduct.style.backgroundColor = "white";
                clickedProduct.style.color = "black";

                individualPrice.style.borderLeft = "1px solid black";
            }
            else if (clickedProduct.className == "addedTicketProduct") {
                clickedProduct.style.backgroundColor = "#D7FCDA";
            }
            else {
                clickedProduct.style.backgroundColor = "rgb(222, 85, 0)";
                clickedProduct.style.color = "white";
            }
        }
        else {
            clickedProduct.style.backgroundColor = "rgb(196, 31, 31)";
        }

        modifyDeleteMenu.style.display = "none";
    }
    else {
        if (clickedProduct.style.backgroundColor !== "rgb(196, 31, 31)") { // Not deleted item?
            if (clickedProduct.className == "ticketProduct") {
                clickedProduct.style.backgroundColor = "rgb(28, 89, 176)";
                clickedProduct.style.color = "white";

                individualPrice.style.borderLeft = "1px solid white";
            }
            else if (clickedProduct.className == "addedTicketProduct") {
                clickedProduct.style.backgroundColor = "#e9fe6d";
            }
            else {
                clickedProduct.style.backgroundColor = "rgb(222, 85, 0)";
                clickedProduct.style.color = "white";
            }
        }
        else {

        }

        if (modifyingProduct == null) {
            modifyDeleteMenu.style.display = "flex";
        }
        else {
            if (modifyingProduct.style.backgroundColor !== "rgb(196, 31, 31)") {
                if (modifyingProduct.className == "ticketProduct") {
                    modifyingProduct.style.backgroundColor = "white"; 
                    modifyingProduct.style.color = "black";
                }
                else if (modifyingProduct.className == "addedTicketProduct") {
                    modifyingProduct.style.backgroundColor = "#D7FCDA";
                    modifyingProduct.style.color = "black";
                }
                else {
                    modifyingProduct.style.backgroundColor = "rgb(222, 85, 0)";
                    if(modifyingProduct === null) clickedProduct.style.color = "white";
                }
            }
            else {
                modifyingProduct.style.backgroundColor = "rgb(196, 31, 31)";
            }
            modifyDeleteMenu.style.display = "flex";
        }

        modifyingProduct = clickedProduct;
    }
}

function selectProductToModify() {
    event.stopPropagation(); // So the child onclick is on top of the parents'

    const modifyProductMenu = document.getElementById("modifyProductMenu");

    modifyProductMenu.style.display = "flex";
    modifyProductMenu.style.zIndex = "6";

    let tab = document.getElementsByClassName("tab");
    let ticketMenu = document.getElementById("ticketMenu");

    tab[0].style.pointerEvents = "none";
    tab[0].style.filter = "blur(10px) grayscale(100%)";
    ticketMenu.style.pointerEvents = "none";
    ticketMenu.style.filter = "blur(10px) grayscale(100%)";
    document.body.style.backgroundColor = "#b0b0b0";
}
function modifyProduct() {
    let amountInput = document.getElementById("amountInput");
    let productTimes = modifyingProduct.children[0].children[0];
    let oldProductTimes = modifyingProduct.children[0].children[1];
    let individualPrice = modifyingProduct.children[3];

    if (amountInput.value === "") {
        amountInput = "0";
    }
    else if (amountInput.value === oldProductTimes.textContent.substr(1) || amountInput.value === productTimes.textContent.substr(1)) {
        modifyingProduct.style.backgroundColor = "rgb(28, 89, 176)";
        modifyingProduct.style.color = "white";
        modifyingProduct.className = "ticketProduct";
        individualPrice.style.borderLeft = "1px solid white";

        if (oldProductTimes.textContent !== "") {
            productTimes.textContent = oldProductTimes.textContent;
            oldProductTimes.style.textDecoration = "none";
            oldProductTimes.textContent = ""
        }     
    }
    else {
        modifyingProduct.style.backgroundColor = "rgb(222, 85, 0)";
        modifyingProduct.style.color = "white";
        individualPrice.style.borderLeft = "1px solid black";

        if (modifyingProduct.children[0].children[1].textContent === "") {
            modifyingProduct.className = "modifiedProduct"; 

            oldProductTimes.textContent = productTimes.textContent;
            oldProductTimes.style.textDecoration = "line-through";
        }

            productTimes.style.textDecoration = "none";
            productTimes.textContent = "x" + amountInput.value;
    }

    amountInput.value = ""; 

    let currency = " " + price.textContent[price.textContent.length - 1];
    const discount = document.getElementById("discountValue").textContent;
    price.textContent = (parseFloat(price.textContent) - (parseFloat(modifyingProduct.children[3].textContent) * (1.0 - parseFloat(discount) / 100.0))).toFixed(2) + currency;

    modifyingProduct.children[3].textContent = (parseFloat(modifyingProduct.children[2].textContent) * parseFloat(productTimes.textContent.substr(1))).toFixed(2);
    price.textContent = (parseFloat(price.textContent) + (parseFloat(modifyingProduct.children[3].textContent) * (1.0 - parseFloat(discount) / 100.0))).toFixed(2) + currency;

    let index = added_ticket.findIndex(prod => prod.name === modifyingProduct.children[1].textContent);
    if (index !== -1) {
        added_ticket[index].times = modifyingProduct.children[0].children[0].textContent.substr(1);
    }

    let modified_prod = {
        new_amount: modifyingProduct.children[0].children[0].textContent.substr(1),
        name: modifyingProduct.children[1].children[0].textContent,
        price: modifyingProduct.children[2].textContent,
        details: modifyingProduct.children[1].children[1].textContent
    };

    modified_ticket.push(modified_prod);

    cancelModifyDeleteMenu();
}

function openAddDetailsMenu() {
    //event.stopPropagation(); // So the child onclick is on top of the parents'
    let lastOrderDetails = document.getElementById("lastOrderDetails");
    let lastOrderDetailsStyle = window.getComputedStyle(lastOrderDetails);
    console.log(lastOrderDetailsStyle.borderTop);
    if (lastOrderDetailsStyle.borderTop === "1px solid rgb(9, 43, 92)") {
        lastOrderDetails.style.borderTop = "1px solid white";
        lastOrderDetails.style.pointerEvents = "auto";
        lastOrderDetails.focus();
    }
    else {
        lastOrderDetails.style.borderTop = "1px solid rgb(9, 43, 92)";
        lastOrderDetails.style.pointerEvents = "none";
        lastOrderDetails.value = "";
        console.log("!a");
    }


    /*if (modifyingProduct.className === "addedTicketProduct" || modifyingProduct.className === "modifiedProduct") {
        let previous_details = modifyingProduct.children[1].children[1].textContent
        modifyingProduct.children[1].children[1].textContent = "Sample";

        let found = added_ticket.find(prod => (prod.name === modifyingProduct.children[1].children[0].textContent) && (prod.details === previous_details));
        if (found) {
            found["details"] = "Sample";
        }
    }*/
}

function addDetails() {

}

function cancelModifyDeleteMenu() {
    const modifyProductMenu = document.getElementById("modifyProductMenu");
    const amountInput = document.getElementById("amountInput");

    modifyProductMenu.style.display = "none";

    let tab = document.getElementsByClassName("tab");
    let ticketMenu = document.getElementById("ticketMenu");

    tab[0].style.pointerEvents = "auto"
    tab[0].style.filter = "blur(0px) grayscale(0%)";
    ticketMenu.style.pointerEvents = "auto";
    ticketMenu.style.filter = "blur(0px) grayscale(0%)";
    document.body.style.backgroundColor = "#bdd7fc";

    amountInput.value = "";
}

function selectProductToDelete(clickedElement) {
    event.stopPropagation(); // So the child onclick is on top of the parents'

    let modifyDeleteMenu = document.getElementById("modifyDeleteMenu");
    let a = document.getElementById("modifyDeleteMenu").parentNode;
    let individualPrice = modifyingProduct.children[3];

    if (a.style.backgroundColor === "rgb(196, 31, 31)") {
        if (a.className === "ticketProduct") {
            a.style.backgroundColor = "rgb(28, 89, 176)";
            a.style.color = "white";
            individualPrice.style.borderLeft = "1px solid white";
        }
        else {
            a.style.backgroundColor = "rgb(222, 85, 0)";
            a.style.color = "white";
            individualPrice.style.borderLeft = "1px solid black";
        }
    }

    else {
        a.style.backgroundColor = "rgb(196, 31, 31)";
        a.style.color = "white";
        individualPrice.style.borderLeft = "1px solid white";
    }

    if (a.className === "ticketProduct") {
        let index = deleted_ticket.findIndex(prod => prod.name === a.children[1].textContent);
        if (index === -1) {
            let prod = {
                times: a.children[0].textContent.substr(1),
                name: a.children[1].children[0].textContent,
                price: a.children[2].textContent,
                details: a.children[1].children[1].textContent
            }

            deleted_ticket.push(prod);
        }
        else {
            deleted_ticket.splice(index, 1);
        }
    }
    else {
        let index = added_ticket.findIndex(prod => prod.name === a.children[1].textContent);
        if (index !== -1) {
            let last = added_ticket[added_ticket.length - 1];
            const price = document.getElementById("price");
            const discount = document.getElementById("discount").textContent;

            let currency = " " + price.textContent[price.textContent.length - 1];
            price.textContent = (parseFloat(price.textContent) - (parseFloat(added_ticket[index].price) * added_ticket[index].times * (1.0 - parseFloat(discount) / 100.0))).toFixed(2) + currency;
            added_ticket.splice(index, 1);

            let addedDivision = document.getElementById("divisionAddedProducts");
            if (added_ticket.length > 0) {
                last = added_ticket[added_ticket.length - 1];
                lastProduct.textContent = "x" + last.times + " " + last.name + " | " + last.price;
            }
            else lastProduct.textContent = "-", addedDivision.style.display = "none";

            modifyDeleteMenu = document.getElementById("modifyDeleteMenu");
            modifyDeleteMenu.style.display = "none";
            document.body.appendChild(modifyDeleteMenu);

            a.remove();
        }
    }
}


function getCookie(cookieName) {
    const name = cookieName + "=";
    const decodedCookie = decodeURIComponent(document.cookie);
    const cookieArray = decodedCookie.split(';');

    for (let i = 0; i < cookieArray.length; i++) {
        let cookie = cookieArray[i];
        while (cookie.charAt(0) === ' ') {
            cookie = cookie.substring(1);
        }
        if (cookie.indexOf(name) === 0) {
            return cookie.substring(name.length, cookie.length);
        }
    }

    return null;
}


function selectIngredientsTab() {
    const ingredientsAndAllergens = document.getElementById("ingredientsAndAllergens");

    const ingredientsTab = document.getElementById("ingredientsTab");
    const ingredients = ingredientsAndAllergens.querySelector(".ingredients");
    const ingredientsList = ingredients.querySelector(".ingredients");
    
    ingredientsTab.style.backgroundColor = "rgb(28, 89, 176)";
    ingredientsTab.style.borderBottom = "0px";
    if (ingredients.children.length > 0 && ingredientsList.children.length > 0) ingredients.style.display = "flex";
    if (ingredients.children.length > 0 && ingredientsList.children.length > 0) ingredientsList.style.display = "flex";

    const allergensTab = document.getElementById("allergensTab");
    const allergens = ingredientsAndAllergens.querySelector(".allergens");
    const allergensList = allergens.querySelector(".allergens");

    allergensTab.style.backgroundColor = "rgb(9, 43, 92)";
    allergensTab.style.borderBottom = "2px solid white";
    if (allergens.children.length > 0 && allergensList.children.length > 0) allergens.style.display = "none";
    if (allergens.children.length > 0 && allergensList.children.length > 0) allergensList.style.display = "none";
}

function selectAllergensTab() {
    const ingredientsAndAllergens = document.getElementById("ingredientsAndAllergens");

    const ingredientsTab = document.getElementById("ingredientsTab");
    const ingredients = ingredientsAndAllergens.querySelector(".ingredients");
    const ingredientsList = ingredients.querySelector(".ingredients");

    ingredientsTab.style.backgroundColor = "rgb(9, 43, 92)";
    ingredientsTab.style.borderBottom = "2px solid white";
    if (ingredients.children.length > 0 && ingredientsList.children.length > 0) ingredients.style.display = "none";
    if (ingredients.children.length > 0 && ingredientsList.children.length > 0) ingredientsList.style.display = "none";

    const allergensTab = document.getElementById("allergensTab");
    const allergens = ingredientsAndAllergens.querySelector(".allergens");
    const allergensList = allergens.querySelector(".allergens");

    allergensTab.style.backgroundColor = "rgb(28, 89, 176)";
    allergensTab.style.borderBottom = "0px";
    if (allergens.children.length > 0 && allergensList.children.length > 0) allergens.style.display = "flex";
    if (allergens.children.length > 0 && allergensList.children.length > 0) allergensList.style.display = "flex";
}