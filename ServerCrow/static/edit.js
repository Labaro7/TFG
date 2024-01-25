// Init
let tabNumber = 1;
let currentGridNumber = 1;
let currentButton = document.getElementById("productsPagesButton" + 1);
currentButton.style.backgroundColor = "#0a3677";
currentButton.style.color = "white";
currentButton.style.border = "1px solid black";
let selectedDeployable = 0;
let selectedPage = 1;

// Modify
let selectedElement; document.getElementById("selected-product");
let selectedElementName;
let selectedElementPrice;
const selectedElementPage = selectedPage;

// Fourth row
let page_number = 1;

function changeTab(clickedTab) {
    const currentPage = document.querySelector('div[data-page="' + tabNumber + '"]');
    let currentTab = document.querySelector("div[data-tab='" + tabNumber + "']");
    currentPage.style.display = "none";
    currentTab.children[1].style.visibility = "hidden";

    tabNumber = clickedTab.getAttribute("data-tab");

    const newPage = document.querySelector('div[data-page="' + tabNumber + '"]');
    currentTab = document.querySelector("div[data-tab='" + tabNumber + "']");
    newPage.style.display = "flex";
    currentTab.children[1].style.visibility = "visible"; 
}

// TODO: It cannot display 6 or more menus
function displayMenu(clickedButton) {
    selectedDeployable = 0;
    selectedPage = clickedButton.textContent;
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

function selectButton(number) {
    let button = document.getElementById("productsPagesButton" + number);
    currentButton.style.backgroundColor = "white";
    currentButton.style.color = "black";
    currentButton.style.border = "1px solid #0a3677";
    currentButton = button;

    button = document.getElementById("productsPagesButton" + number);
    button.style.backgroundColor = "#0a3677";
    button.style.color = "white";
    currentButton.style.border = "1px solid black";
}

function openDeployable(clickedDeployable) {
    selectElement(clickedDeployable);

    selectedDeployable = clickedDeployable.dataset.id;
    const grid = "grid" + page_number;
    const grid_products = document.getElementById(grid).querySelectorAll("li");
    for (let i of grid_products) {
        i.style.display = "none";
    }

    const clickedDeployableProducts = document.querySelectorAll("li[data-name='" + clickedDeployable.textContent);
    for (let i of clickedDeployableProducts) {
        i.style.display = "flex";
    }
}

function addProduct() {
    const name = document.getElementById("nameInput").value;
    let price = document.getElementById("priceInput").value;
    const color = document.getElementById("colorInput").value;
    let deployable = document.getElementById("deployableInput").value;
    
    if (( (name && price != 0) && (name && price != 0.0)) || (name && deployable == "Deployable")) {
        deployable == "Deployable" ? (deployable = 0, price = "0") : deployable = selectedDeployable;

        const data = {
            name: name,
            price: price,
            color: color,
            page: selectedPage,
            deployable: deployable,
        };

        const url = "/edit/add/product";
        fetch(url, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(data),
        })
            .then(response => response.text())
            .then(data => {
                console.log('Response:', data);
            })
            .catch(error => {
                console.error('Error:', error);
            });

        setTimeout(() => { window.location.href = "/edit"; }, 100);
    }
    else {
        alert("Complete the necessary fields");
    }
}

function selectElement(clickedElement) {
    const clonedElement = clickedElement.cloneNode(true);
    clonedElement.id = "selected-product";
    clonedElement.className = "";

    selectedElement = document.getElementById("selectedElement");

    selectedElement.innerHTML = "";
    selectedElement.appendChild(clonedElement);

    if (selectedElement.children[0].hasAttribute("data-id")) {
        selectedElementName = selectedElement.textContent;
        selectedElementPrice = "0";
        newPrice = "0";
    }
    else {
        selectedElementName = selectedElement.children[0].children[0].textContent;
        selectedElementPrice = selectedElement.children[0].children[1].textContent;
    }
}

function modifyProduct() {
    let newName = document.getElementById("newNameInput").value;
    let newPrice = document.getElementById("newPriceInput").value;
    const newColor = document.getElementById("newColorInput").value;

    if (newName !== "") {
        const data = {
            selectedElementName: selectedElementName,
            selectedElementPrice: selectedElementPrice,
            selectedElementPage: selectedElementPage,
            newName: newName,
            newPrice: newPrice,
            newColor: newColor
        };

        // TODO: Load page when response is received
        const url = "/edit/modify/product";
        fetch(url, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(data),
        })
            .then(response => {
                if (response.ok) setTimeout(() => { window.location.href = "/edit"; }, 100);
            })
            .then(data => {
                console.log('Response:', data);
            })
            .catch(error => {
                console.error('Error:', error);
            });
    }
    else {
        alert("Complete the necessary fields");
    }
}

function deleteProduct() {
    let data = {
        selectedElementName: selectedElementName,
        selectedElementPrice: selectedElementPrice,
        selectedElementPage: selectedElementPage
    }

    const url = "/edit/delete/product";
    fetch(url, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(data),
    })
        .then(response => {
            if (response.ok) setTimeout(() => { window.location.href = "/edit"; }, 100);
        })
        .then(data => {
            console.log('Response:', data);
        })
        .catch(error => {
            console.error('Error:', error);
        });
}