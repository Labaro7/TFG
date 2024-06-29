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

async function login() {
    try {
        let data = {
            username: username.value,
            password: password.value,
        }

        const url = '/login';
        console.log("c");
        let response = await fetch(url, {
            method: 'POST',
            headers: {
                'Content-Type': 'text/html',
            },
            body: JSON.stringify(data),
        })
            .catch(error => {
                console.error('Error:', error);
            });

        console.log("m");
        let result = await response.text();
        console.log(result);    
        if (response.ok) {
            window.location.replace('/');
        }


        //setTimeout(() => { window.location.replace("/"); }, 100);
    } catch (error) {
        console.error('Error:', error);
    }

}