export default class HeaderView {

    #body = async () => /*html*/`
        <figure id="profile-trigger" class="selectable">
            <img src="./assets/account.svg" />
            <figcaption>nagibator_2007</figcaption>
        </figure>
        <figure id="search-trigger" class="selectable">
            <img src="./assets/search.svg" />
            <figcaption>Search</figcaption>
        </figure>
        <hr />
    `

    async render() {
        this.container.innerHTML = await this.#body();
    }

    constructor(container) {
        this.container = container;
    }
}
