const selectTableTab = document.getElementById("selectTableTab");
const currentTablesTab = document.getElementById("currentTablesTab");
const selectTablePage = document.getElementById("select-table-page");
const currentTablesPage = document.getElementById("current-tables-page");

function changeToSelectTableTab() {
    selectTableTab.style.backgroundColor = 'rgb(9, 43, 92)';
    selectTableTab.style.color = "white";
    currentTablesTab.style.backgroundColor = 'white';
    currentTablesTab.style.color = "black";

    showPage('select-table-page');
}

function changeToCurrentTablesTab() {
    selectTableTab.style.backgroundColor = 'white';
    selectTableTab.style.color = "black";
    currentTablesTab.style.backgroundColor = 'rgb(9, 43, 92)';
    currentTablesTab.style.color = "white";

    showPage('current-tables-page');
}

// Function to show the specified page
function showPage(pageId) {
    // Hide all pages
    const pages = document.querySelectorAll('.page');
    pages.forEach(page => {
        page.style.display = 'none';
    });

    // Show the selected page
    const selectedPage = document.getElementById(pageId);
    if (selectedPage) {
        selectedPage.style.display = 'flex';
    }
}

// Function to append a number to the display
function appendNumber(number) {
    const display = document.getElementById('tableInput');

    if (display.value === "0") display.value = "";

    if (display.value <= 999) display.value += number;
}

// Function to append a dot to the display
function appendDot() {
    const display = document.getElementById('tableInput');
    display.value += '.';
}

// Function to clear the display
function clearDisplay() {
    const display = document.getElementById('tableInput');
    display.value = "";
}

function goToTable(clickedTable) {
    let n_table = (clickedTable.children)[1].textContent.substring(1);

    let url = "/table?tableInput=" + n_table;

    fetchData(url)
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }

            return response.text();
        })
        .then(data => {
            console.log('Data received:', data);
        })
        .catch(error => {
            console.error('Fetch error:', error);
        });
}


