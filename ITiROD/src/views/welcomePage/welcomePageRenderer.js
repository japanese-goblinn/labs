export default class WelcomePageRenderer {

    #body = async () => /*html*/`
        <header>
            <h1>Welcome to WebNotes!</h1>
        </header>
        <main>
            <div id="sign-in-form" class="form-container">
                <h2>Sign In</h2>
                <hr>
                <form class="form">
                    <label for="si-email">Email</label>
                    <input id="si-email" type="email" />
                    <label for="si-password">Password</label>
                    <input id="si-password" type="password" />
                    <button id="show-sign-up" class="change-button" type="button">No profile?</button>
                    <button class="primary-button">Sign In</button>
                </form>
            </div>
            <div id="sign-up-form" class="form-container" style="display: none;">
                <h2>Sign Up</h2>
                <hr>
                <form class="form">
                    <label for="su-email">Email</label>
                    <input id="su-email" type="email" />
                    <label for="su-password">Password</label>
                    <input id="su-password" type="password" />
                    <label for="repeat-password">Repeat password</label>
                    <input id="repeat-password" type="password" />
                    <button id="show-sign-in" class="change-button" type="button">Already signed up?</button>
                    <button class="primary-button">Sign Up</button>
                </form>
            </div>
        </main>
    `

    #configure = () => {
        const signInForm = document.getElementById('sign-in-form');
        const signUpForm = document.getElementById('sign-up-form');
        const showSignIn = document.getElementById('show-sign-in');
        const showSignUp = document.getElementById('show-sign-up');

        showSignIn.addEventListener('click', () => {
            signInForm.style.display = "block";
            signUpForm.style.display = "none";
        });

        showSignUp.addEventListener('click', () => {
            signUpForm.style.display = "block";
            signInForm.style.display = "none";
        });
    }

    async render() {
        document.getElementById('root').innerHTML = await this.#body();
        this.#configure();
    }

}
