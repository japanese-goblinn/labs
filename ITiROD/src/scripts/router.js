import { renderer, mobile } from "../app.js";
import Auth from "./auth.js";

export default class Router {

    #routes = {
        '/': 'MainPageView',
        '/welcome': 'WelcomePageView',
        '/folder/:id': 'NotesColumnView',
        '/folder/:id/note/:id': 'NoteView',
        '/404': '404'
    };

    get currentView() {
        const path = this._matchURL(this._splitCurrentURL());
        return this.#routes[path];
    }

    _splitCurrentURL() {
        return this._splitURL(window.location.pathname);
    }

    _splitURL(url) {
        return url.split('/').filter(l => l);
    }

    _matchURL(splittedURL) {
        switch (splittedURL.length) {
            case 0:
                return '/';
            case 1:
                return '/' + splittedURL[0];
            case 2:
                return '/folder/:id';
            case 4:
                return '/folder/:id/note/:id';
            default:
                return '/404';
        }
    }

    _changeURL(newURL) {
        const pathSplitted = this._splitURL(newURL);
        switch (pathSplitted[0]) {
            case 'folder':
                window.history.replaceState(null, null, '/');
                window.history.pushState(null, null, newURL);
                break;
            case 'note':
                const replace = this._splitCurrentURL().slice(0, 2).join('/');
                window.history.replaceState(null, null, '/');
                window.history.pushState(null, null, replace + '/');
                window.history.pushState(null, null, newURL);
                break;
            default:
                window.history.pushState(null, null, newURL);
        }
    }

    dataFromURL() {
        const splittedURL = this._splitCurrentURL();
        switch (splittedURL.length) {
            case 2:
                return {
                    folderID: splittedURL[1]
                };
            case 4:
                return {
                    folderID: splittedURL[1],
                    noteID: splittedURL[3]
                };
            default:
                return null;
        }
    }

    async refresh() {
        const splitted = this._splitCurrentURL();
        window.history.replaceState(null, null, '/');
        switch (splitted.length) {
            case 2:
                await this.navigate('/');
                await this.navigate('folder/' + splitted[1]);
                break;
            case 4:
                await this.navigate('/');
                if (mobile.matches) {
                    this._changeURL('folder/' + splitted[1]);
                } else {
                    await this.navigate('folder/' + splitted[1]);
                }
                await this.navigate('note/' + splitted[3]);
                break;
            default:
                await this.navigate('/');
                break;
        }
    }

    async navigate(path) {
        this._changeURL(path);
        await this.render();
    }

    async render() {
        await renderer.render(this.currentView);
    }

    constructor() {
        window.history.replaceState(null, null, '/');
        window.addEventListener('popstate', async () => {
            await this.render();
        });
        mobile.addListener(async () => {
            if (!Auth.isSignedIn()) {
                return
            }
            await this.refresh();
        });
        window.onpopstate = () => {
            const spitted = this._splitCurrentURL();
            if (spitted[0] === 'welcome') {
                this._changeURL('/welcome');
            } else if (spitted[0] == undefined) {
                this._changeURL('/');
            }
        }
    }
}
