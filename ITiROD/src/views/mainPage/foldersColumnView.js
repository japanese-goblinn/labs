export default class FoldersColumnView {

    #body = async () => /*html*/`
        <nav>
            <ul class="list folder-list">
                <li class="dropdown selectable">
                    <div class="folder-description">
                        <h4>Loooooooooooooong folder</h4>
                        <p class="secondary">Description wow wow wow wowwowowowowo</p>
                    </div>
                    <button class="folder-button rounded selectable dropdown-trigger">
                        ...
                    </button>
                    <div class="dropdown-container">
                        <button class="secondary selectable">Edit</button>
                        <button class="destructive selectable">Delete</button>
                    </div>
                </li>
            </ul>
        </nav>
        <button id="folder-create-trigger" class="primary-button">+ New folder</button>
    `

    async render() {
        this.container.innerHTML = await this.#body();
    }

    constructor(container) {
        this.container = container;
    }
}
