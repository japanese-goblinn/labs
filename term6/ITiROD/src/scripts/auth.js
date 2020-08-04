import { router } from "../app.js";

export default class Auth {

    static _showError(error) {
        alert(`Error. ${error.message}`);
    }

    static updatePassword(newPassword) {
        this.currentUser.updatePassword(newPassword).then(() => {
            this.signOut()
        }).catch(this._showError);
    }

    static updateEmail(newEmail) {
        this.currentUser.updateEmail(newEmail).then(async () => {
            this.signOut();
        }).catch(this._showError);
    }

    static signUp(email, password) {
        firebase.auth().createUserWithEmailAndPassword(email, password).then(async () => {
            await router.navigate('/');
        }).catch(this._showError);
    }

    static signIn(email, password) {
        firebase.auth().signInWithEmailAndPassword(email, password).then(async () => {
            await router.navigate('/');
        }).catch(this._showError);
    }

    static signOut() {
        firebase.auth().signOut().then(async () => {
            await router.navigate('/welcome');
        }).catch(this._showError);
    }

    static isSignedIn() {
        return firebase.auth().currentUser ? true : false;
    }

    static get currentUser() {
        return firebase.auth().currentUser;
    }

    static init() {
        firebase.auth().onAuthStateChanged(async (user) => {
            if (!user) {
                await router.navigate('/welcome');
                return
            } 
            await router.navigate('/');
        });
    }
}
