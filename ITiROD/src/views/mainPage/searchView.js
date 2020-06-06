export default class SearchView {

    #body = async () => /*html*/`
        <div class="modal-content">
            <div class="search">
                <div class="search-bar">
                    <img src="../assets/search.svg" />
                    <input type="search" placeholder="Search..." />
                </div>
                <hr />
            </div>
            <ul class="list">
                <li class="folder-description selectable">
                    <h4>Loooooooooooooong folder</h4>
                    <p class="secondary">Description wow wow wow wowwowowowowo</p>
                    <p class="search-element-type secondary">Folder</p>
                </li>
                <li class="note-description selectable">
                    <h4>Nooooooooooooooooooote 1</h4>
                    <p class="search-element-type secondary">Note</p>
                </li>
            </ul>
        </div>
    `

    async render() {
        this.container.innerHTML = await this.#body();
    }

    constructor(container) {
        this.container = container;
    }
}
