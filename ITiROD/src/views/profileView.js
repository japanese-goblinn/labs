import { router } from '../../app.js';

export default class ProfileView {

    #body = async () => /*html*/`
        <div class="modal-content">
            <figure>
                <img src="../assets/account-big.svg" />
                <figcaption>${'Kirill'}</figcaption>
            </figure>
            <hr />
            <div id="edit-form" class="form-container" style="display: none;">
                <form class="form">
                    <label for="email">New email</label>
                    <input id="email" type="email" />
                    <label for="old-password">Old Password</label>
                    <input id="old-password" type="password" />
                    <label for="new-password">New password</label>
                    <input id="new-password" type="password" />
                    <label for="repeat-new-password">Repeat new password</label>
                    <input id="repeat-new-password" type="password" />
                </form>
            </div>
            <div class="buttons-container">
                <button id="save-changes-button" class="primary-button" style="display: none;">Save changes</button>
                <button id="edit-button" class="primary-button">Edit profile</button>
                <button id="sign-out-button" class="sign-out-button">Sign Out</button>
            </div>
        </div>
    `

    #configure = async () => {
        const b = document.getElementById('sign-out-button');
        b.addEventListener('click', async () => {
            await router.navigate('/404');
        });

        const editButton = document.getElementById('edit-button');
        const editForm = document.getElementById('edit-form');
        const saveChangesButton = document.getElementById('save-changes-button');
        editButton.addEventListener('click', () => {
            editForm.style.display = "flex";
            editButton.style.display = "none";
            saveChangesButton.style.display = "inline";
        });
        saveChangesButton.addEventListener('click', () => {
            editForm.style.display = "none";
            saveChangesButton.style.display = "none";
            editButton.style.display = "inline";
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
