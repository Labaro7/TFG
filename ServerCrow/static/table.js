function changeToProductsTab() {
    const ProductsTab = document.getElementById("tab-buttonProducts");
    const TicketTab = document.getElementById("tab-buttonTicket");
    ProductsTab.style.visibility = 'visible';
    TicketTab.style.visibility = 'hidden';
    showPage('Box');
}

function changeToTicketTab() {
    const ProductsTab = document.getElementById("tab-buttonProducts");
    const TicketTab = document.getElementById("tab-buttonTicket");
    ProductsTab.style.visibility = 'hidden';
    TicketTab.style.visibility = 'visible';
    showPage('Ticket');
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