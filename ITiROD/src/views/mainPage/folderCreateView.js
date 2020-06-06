export default class FolerCreateView {

    #body = async () => /*html*/`
        <div class="modal-content">
            <div id="folder-create-form" class="form-container">
                <form class="form">
                    <label for="folder-name">Folder name</label>
                    <input id="folder-name" type="text" />
                    <label for="folder-description">Folder desctiption</label>
                    <input id="folder-description" type="text" />
                    <button id="confirm-new-folder-button" class="primary-button" type="button">Create</button>
                </form>
            </div>
        </div>  
    `

    async render() {
        this.container.innerHTML = await this.#body();
    }

    constructor(container) {
        this.container = container;
    }
}
