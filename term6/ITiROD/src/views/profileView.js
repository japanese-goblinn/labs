import Auth from "../scripts/auth.js";

export default class ProfileView {

    #body = async (email) => /*html*/`
        <div class="modal-content">
            <figure>
                <img src="../assets/account-big.svg" />
                <figcaption>${email}</figcaption>
            </figure>
            <hr />
            <div id="edit-form" class="form-container" style="display: none;">
                <form id="account-edit-form" class="form">
                    <label for="email">New email</label>
                    <input id="email" type="email" placeholder="${email}"/>
                    <label for="new-password">New password</label>
                    <input id="new-password" type="password" />
                    <label for="repeat-new-password">Repeat new password</label>
                    <input id="repeat-new-password" type="password" />
                </form>
            </div>
            <div class="buttons-container">
                <button id="save-changes-button" form="account-edit-form" class="primary-button" style="display: none;">
                    Save changes
                </button>
                <button id="edit-button" class="primary-button">Edit profile</button>
                <button id="sign-out-button" class="sign-out-button">Sign Out</button>
            </div>
        </div>
    `

    #configure = async () => {
        const signOut = document.getElementById('sign-out-button');
        signOut.addEventListener('click', () => Auth.signOut());

        const editButton = document.getElementById('edit-button');
        const editForm = document.getElementById('edit-form');
        const saveChangesButton = document.getElementById('save-changes-button');
        editButton.addEventListener('click', () => {
            editForm.style.display = 'flex';
            editButton.style.display = 'none';
            saveChangesButton.style.display = 'inline';
        });

        const accountEditForm = document.getElementById('account-edit-form');
        accountEditForm.addEventListener('submit', (event) => {
            event.preventDefault();

            const newEmailValue = document.getElementById('email').value;
            const newPasswordValue = document.getElementById('new-password').value;
            const newPasswordRepetedValue = document.getElementById('repeat-new-password').value;  
            
            if (newPasswordValue !== '' && newEmailValue !== '') {
                alert('Can\'t change password and email at the same time');
                return;
            }
            
            if (newEmailValue) {
                Auth.updateEmail(newEmailValue);
                return
            }

            if (newPasswordValue === '' && newPasswordRepetedValue === '') {
                return
            }
            if (newPasswordValue != newPasswordRepetedValue) {
                alert('New and repeated new passwords do not match');
                return;
            }
            Auth.updatePassword(newPasswordValue);
        });
    }

    async render() {
        this.container.innerHTML = await this.#body(Auth.currentUser.email);
        await this.#configure();
    }

    constructor(container) {
        this.container = container;
    }
}
