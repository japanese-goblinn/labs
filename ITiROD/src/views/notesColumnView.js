import animate from "../../scripts/animate.js";
import { router, renderer, mobile } from "../app.js";

export default class NotesColumnView {

    #body = async () => /*html*/`
        <button id="folder-back-button" class="back-button folder-back-button selectable">
            <img src="../../../assets/back.svg" alt="Back" />
        </button>
        <section class="notes-header">
            <div class="notes-header-description">
                <h3>Loooooooooooooong folder</h3>
                <p class="secondary">Description wow wow wow wow</p>
            </div>
            <button id="new-note-button" class="primary-button rounded">+</button>
        </section>
        <hr />
        <nav>
            <ul class="list note-list">
                <li id="n-t" class="dropdown selectable">
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
                <li id="n-t2" class="dropdown selectable">
                    <div class="note-description">
                        <h5>Nooooooooooooooooooote 2</h5>
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

    #configure = async () => {
        const li = document.getElementById('n-t');
        li.addEventListener('click', async () => {
            await router.navigate('note/' + 'n-t');
        });

        const li2 = document.getElementById('n-t2');
        li2.addEventListener('click', async () => {
            await router.navigate('note/' + 'n-t2');
        });

        const folderBack = document.getElementById('folder-back-button');
        folderBack.addEventListener('click', () => window.history.back());
        folderBack.style.display = "none";

        this.container.classList.add('right-bordered');
        
        await renderer.renderEmpty('NoteView');

        const newNoteButton = document.getElementById('new-note-button');
        animate(newNoteButton, 'primary-button-click-animation');

        if (mobile.matches) {
            folderBack.style.display = 'inline-block';
        } else {
            folderBack.style.display = 'none';
        }
    }

    async render() {
        this.container.innerHTML = await this.#body();
        await this.#configure();
    }

    constructor(container) {
        if (mobile.matches) {
            this.container = document.getElementById('folders-column-container');
            this.container.classList.remove(...this.container.classList);
            this.container.classList.add(container.className);
        } else {
            this.container = container;
        }
    }
}
