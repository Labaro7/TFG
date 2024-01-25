const username = document.getElementById("username");
const password = document.getElementById("password");
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
    console.log("c");
    fetch(url, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(data),
    })
        .then(response => {
            const cookieHeader = response.headers.get('Set-Cookie');

            if (cookieHeader) {
                document.cookie = cookieHeader;

                console.log("a1");
                setTimeout(() => { window.location.href = "/"; }, 100);
                console.log("a2");
            } else {
                console.log("No 'Set-Cookie' header in the response");
            }


        })
        .catch(error => {
            console.error('Error:', error);
        });
}