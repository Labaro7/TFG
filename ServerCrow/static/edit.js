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
let selectedElementPage = selectedPage;

// Fourth row
let page_number = 1;

// Employee
let selectedEmployee = {
    firstName: "",
    lastName: "",
    email: "",
    id: "",
    mobileNumber: "",
    level: "",
    username: "",
    password_hash: "",
    session_token: ""
};
let selectedEmployeeElement;
let oldEmployee_firstName;
let oldEmployee_lastName;
let oldEmployee_email;
let oldEmployee_id;
let oldEmployee_mobile;
let oldEmployee_level;
let oldEmployee_username;
let oldEmployee_password;
let oldEmployee_session_token;



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
    selectedElementPage = selectedPage;
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

    if (((name && price != 0) && (name && price != 0.0)) || (name && deployable == "Deployable")) {
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


/* EMPLOYEE */
const SELECTED_BACKGROUND_COLOR = "#0a3677";
const SELECTED_TEXT_COLOR = "white";
const UNSELECTED_BACKGROUND_COLOR = "white"
const UNSELECTED_TEXT_COLOR = "rgb(20, 20, 51)";
function selectEmployee(clickedEmployee) {
    let detailsInputArray = [
        document.getElementById("firstName"),
        document.getElementById("lastName"),
        document.getElementById("email"),
        document.getElementById("id"),
        document.getElementById("mobileNumber"),
        document.getElementById("level"),
        document.getElementById("username"),
        document.getElementById("password_hash"),
        document.getElementById("session_token")
    ];

    if (selectedEmployeeElement) {
        selectedEmployeeElement.style.backgroundColor = UNSELECTED_BACKGROUND_COLOR;
        selectedEmployeeElement.style.color = UNSELECTED_TEXT_COLOR;
    }

    if (selectedEmployeeElement === clickedEmployee) {
        selectedEmployeeElement.style.backgroundColor = UNSELECTED_BACKGROUND_COLOR;
        selectedEmployeeElement.style.color = UNSELECTED_TEXT_COLOR;
        selectedEmployeeElement = null;

        for (let input of detailsInputArray) input.value = "";
        for (let prop in selectedEmployee) selectedEmployee[prop] = "";
    }
    else {
        selectedEmployeeElement = clickedEmployee;
        selectedEmployeeElement.style.backgroundColor = SELECTED_BACKGROUND_COLOR;
        selectedEmployeeElement.style.color = SELECTED_TEXT_COLOR;

        let i = 0;
        if (clickedEmployee.children[0].children[1]) {
            for (let input of detailsInputArray) {
                input.value = clickedEmployee.children[0].children[1].children[i].textContent;
                i++;
            }

            i = 0;
            for (let prop in selectedEmployee) {
                selectedEmployee[prop] = clickedEmployee.children[0].children[1].children[i].textContent;
                i++;
            }
        }
        else {
            for (let input of detailsInputArray) input.value = "";
        }
    }

    const selectedEmployeeDetails = document.getElementById("selectedEmployeeDetails").children;
    let i = 0;
    for (let div of selectedEmployeeDetails) {
        div.children[0].value = selectedEmployee[div.children[0].id];
        i++;
    }
}
function addEmployee() {
    let employeeList = document.getElementById("employeeList");
    let newEmployee = document.createElement("li");
    let ul = document.createElement("ul");
    let employeeName = document.createElement("li");
    let employeeDetails = document.createElement("ul");

    newEmployee.appendChild(ul);

    employeeName.className = "employeeName";
    newEmployee.appendChild(employeeName);

    employeeDetails.className = "employeeDetails";
    newEmployee.appendChild(employeeDetails);

    newEmployee.className = "employee";
    newEmployee.onclick = function () { selectEmployee(newEmployee); };

    employeeList.appendChild(newEmployee);
}

function addEmployeeDetails() {
    let oldEmployee = {
        firstName: selectedEmployee.firstName ? selectedEmployee.firstName : "",
        lastName: selectedEmployee.lastName ? selectedEmployee.lastName : "",
        email: selectedEmployee.email ? selectedEmployee.email : "",
        id: selectedEmployee.id ? selectedEmployee.id : "",
        mobileNumber: selectedEmployee.mobileNumber ? selectedEmployee.mobileNumber : "",
        level: selectedEmployee.level ? selectedEmployee.level : 0,
        username: selectedEmployee.username ? selectedEmployee.username : "",
        password: selectedEmployee.password_hash ? selectedEmployee.password_hash : "",
        session_token: selectedEmployee.session_token ? selectedEmployee.session_token : ""
    }

    let newEmployee = {
        firstName: document.getElementById("firstName").value,
        lastName: document.getElementById("lastName").value,
        email: document.getElementById("email").value,
        id: document.getElementById("id").value,
        mobileNumber: document.getElementById("mobileNumber").value,
        level: parseInt(document.getElementById("level").value),
        username: document.getElementById("username").value,
        password: document.getElementById("password_hash").value,
        session_token: document.getElementById("session_token").value
    }

    let data = {
        oldEmployee: oldEmployee,
        newEmployee: newEmployee
    }

    const url = "/edit/add/employee";
    fetch(url, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(data),
    })
        .then(response => {
            setTimeout(() => { window.location.href = "/edit"; }, 100);
        })
        .then(data => {
            console.log('Response:', data);
        })
        .catch(error => {
            console.error('Error:', error);
        });
}

function deleteEmployeeDetails() {
    let oldEmployee = {
        firstName: selectedEmployee.firstName ? selectedEmployee.firstName : "",
        lastName: selectedEmployee.lastName ? selectedEmployee.lastName : "",
        email: selectedEmployee.email ? selectedEmployee.email : "",
        id: selectedEmployee.id ? selectedEmployee.id : "",
        mobileNumber: selectedEmployee.mobileNumber ? selectedEmployee.mobileNumber : "",
        level: selectedEmployee.level ? selectedEmployee.level : 0,
        username: selectedEmployee.username ? selectedEmployee.username : "",
        password: selectedEmployee.password_hash ? selectedEmployee.password_hash : "",
        session_token: selectedEmployee.session_token ? selectedEmployee.session_token : ""
    }

    let data = {
        oldEmployee: oldEmployee
    }

    const url = "/edit/delete/employee";
    fetch(url, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(data),
    })
        .then(response => {
            setTimeout(() => { window.location.href = "/edit"; }, 100);
        })
        .then(data => {
            console.log('Response:', data);
        })
        .catch(error => {
            console.error('Error:', error);
        });
}