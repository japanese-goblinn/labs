import Auth from "../scripts/auth.js";

export default class HeaderView {

    #body = async (email) => /*html*/`
        <figure id="profile-trigger" class="selectable">
            <img src="./assets/account.svg" />
            <figcaption>${email}</figcaption>
        </figure>
        <figure id="search-trigger" class="selectable">
            <img src="./assets/search.svg" />
            <figcaption>Search</figcaption>
        </figure>
        <hr />
    `

    async render() {
        this.container.innerHTML = await this.#body(Auth.currentUser.email);
    }

    constructor(container) {
        this.container = container;
    }
}
