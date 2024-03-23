const ctx = document.getElementById('myChart');

function extractBillValues(jsonData) {
    // Parse the JSON data
    const data = JSON.parse(jsonData);

    // Initialize an array to store bill values
    const billValues = [];

    // Iterate through each order and extract the bill value
    data.orders.forEach(order => {
        billValues.push(order.bill);
    });

    // Return the array of bill values
    return billValues;
}

function renderData(data) {
    var ctx = document.getElementById('myChart');

    new Chart(ctx, {
        type: 'line',
        data: {
            labels: data.map((_, index) => index.toString()),
            datasets: [{
                label: 'Data',
                data: data,
                fill: false,
                borderColor: 'rgb(75, 192, 192)',
                tension: 0.1
            }]
        },
        options: {
            scales: {
                x: {
                    display: true,
                    title: {
                        display: true,
                        text: 'Index'
                    }
                },
                y: {
                    display: true,
                    title: {
                        display: true,
                        text: 'Value'
                    }
                }
            }
        }
    });
}

function receiveData() {
    const url = "/api/stats";
    fetch(url, {
        method: 'GET',
        headers: {
            'Content-Type': 'application/json',
        },
    })
        .then(response => response.text())
        .then(data => {
            console.log('Response:');

            renderData(extractBillValues(data));
        })
        .catch(error => {
            //console.error('Error:', error);
        });
}