export default class NotesColumnView {

    #body = async () => /*html*/`
        <section class="notes-header">
            <div class="notes-header-description">
                <h3>Loooooooooooooong folder</h3>
                <p class="secondary">Description wow wow wow wow</p>
            </div>
            <button class="primary-button rounded">+</button>
        </section>
        <hr />
        <nav>
            <ul class="list note-list">
                <li class="dropdown selectable">
                    <div class="note-description">
                        <h5>Nooooooooooooooooooote 1</h5>
                    </div>
                    <button class="note-button rounded dropdown-trigger">
                        ...
                    </button>
                    <div class="dropdown-container">
                        <button class="secondary selectable">Move to</button>
                        <button class="destructive selectable">Delete</button>
                    </div>
                </li>
            </ul>
        </nav>
    `

    async render() {
        this.container.innerHTML = await this.#body();
    }

    constructor(container) {
        this.container = container;
    }
}
