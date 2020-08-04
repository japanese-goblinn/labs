import Auth from "../scripts/auth.js";

export default class WelcomePageView {

    #body = async () => /*html*/`
        <header>
            <h1>Welcome to WebNotes!</h1>
        </header>
        <main>
            <div id="sign-in-form" class="form-container">
                <h2>Sign In</h2>
                <hr>
                <form class="form">
                    <label for="sign-in-email">Email</label>
                    <input id="sign-in-email" type="email" required />
                    <label for="sign-in-password">Password</label>
                    <input id="sign-in-password" type="password" required />
                    <button id="show-sign-up" class="change-button" type="button">No profile?</button>
                    <button class="primary-button">Sign In</button>
                </form>
            </div>
            <div id="sign-up-form" class="form-container" style="display: none;">
                <h2>Sign Up</h2>
                <hr>
                <form class="form">
                    <label for="sign-up-email">Email</label>
                    <input id="sign-up-email" type="email" placeholder="example@smthing.com" required />
                    <label for="sign-up-password">Password</label>
                    <input id="sign-up-password" type="password" required />
                    <label for="sign-up-repeat-password">Repeat password</label>
                    <input id="sign-up-repeat-password" type="password" required />
                    <button id="show-sign-in" class="change-button" type="button">Already signed up?</button>
                    <button class="primary-button">Sign Up</button>
                </form>
            </div>
        </main>
    `

    #configure = async () => {
        document.title = 'Welcome to WebNotes!';
        document.styleSheets[0].disabled = true;
        document.styleSheets[1].disabled = false;
        
        const signInForm = document.getElementById('sign-in-form');
        const signUpForm = document.getElementById('sign-up-form');
        const showSignIn = document.getElementById('show-sign-in');
        const showSignUp = document.getElementById('show-sign-up');
        showSignIn.addEventListener('click', () => {
            signInForm.style.display = 'block';
            signUpForm.style.display = 'none';
        });
        showSignUp.addEventListener('click', () => {
            signUpForm.style.display = 'block';
            signInForm.style.display = 'none';
        });

        signInForm.addEventListener('submit', (event) => {
            event.preventDefault();
            const emailValue = document.getElementById('sign-in-email').value;
            const passwordValue = document.getElementById('sign-in-password').value;
            Auth.signIn(emailValue, passwordValue);
        });
        signUpForm.addEventListener('submit', (event) => {
            event.preventDefault();
            const emailValue = document.getElementById('sign-up-email').value;
            const passwordValue = document.getElementById('sign-up-password').value;
            const repetedPasswordValue = document.getElementById('sign-up-repeat-password').value;
            if (passwordValue != repetedPasswordValue) {
                alert('Passwords do not match');
                return;
            }
            Auth.signUp(emailValue, passwordValue);
        });
    }

    async render() {
        this.container.innerHTML = await this.#body();
        await this.#configure();
    }

    constructor(container) {
        this.container = container;
    }
}
