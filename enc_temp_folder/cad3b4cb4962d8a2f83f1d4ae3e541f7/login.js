const username = document.getElementById("username");
const password = document.getElementById("password");

console.log(document.cookie);
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

function login() {
    let data = {
        username: username.value,
        password: password.value,
    }

    const url = 'https://192.168.1.66:18080/login';
    fetch(url, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(data),
    })
        .then(response => {
            console.log('Response:', response);

            const cookieHeader = response.headers.get('Set-Cookie');

            console.log(response.headers);

            if (cookieHeader) {
                document.cookie = cookieHeader;

                console.log("Cookies set:", document.cookie);
                setTimeout(() => { window.location.href = "https://192.168.1.66:18080"; }, 100);
            } else {
                console.log("No 'Set-Cookie' header in the response");
            }
        })
        .then(data => {

            console.log(data);
            //getCookie();

        })
        .catch(error => {
            console.error('Error:', error);
        });
}