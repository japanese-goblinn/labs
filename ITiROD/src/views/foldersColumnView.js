import { router, mobile } from '../app.js';
import animate from '../scripts/animate.js';

export default class FoldersColumnView {

    #body = async () => /*html*/`
        <nav>
            <ul class="list folder-list">
                <li id="f-t" class="dropdown selectable">
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
                <li id="f-t2" class="dropdown selectable">
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

    #configure = async () => {
        const li = document.getElementById('f-t');
        li.addEventListener('click', async () => {
            await router.navigate('folder/' + 'f-t');              
        });
        const li2 = document.getElementById('f-t2');
        li2.addEventListener('click', async () => {
            await router.navigate('folder/' + 'f-t2');              
        });

        const folderCreate = document.getElementById('folder-create-trigger');
        animate(folderCreate, 'primary-button-click-animation');
    }

    async render() {
        this.container.innerHTML = await this.#body();
        await this.#configure();
    }

    constructor(container) {
        const containerName = container.className;
        this.container = container;
        this.container.classList.remove(...this.container.classList);
        this.container.classList.add(containerName);
    }
}
