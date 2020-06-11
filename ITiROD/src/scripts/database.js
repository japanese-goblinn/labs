import Auth from "./auth.js";
import { router } from "../app.js";

export default class Database {

    static _showError(error) {
        alert(`Error. ${error.message}`);
    }
    
    static async saveFolder(title, description) {
        const newFolderRef = this.db.ref('users/' + Auth.currentUser.uid + '/folders').push();
        newFolderRef.set({
            title: title,
            description: description
        });
        return newFolderRef.key;
    }

    static async deleteFolder(id, title) {
        const ref = this.db.ref('users/' + Auth.currentUser.uid + '/folders/' + id);
        ref.remove().then(() => {
            setTimeout(() => alert(`${title} deleted`), 50);
        }).catch(this._showError);
    }

    static async loadFolder(id) {
        const snapshot = await this.db.ref('users/' + Auth.currentUser.uid + '/folders/' + id).once('value');
        return {
            id: snapshot.key,
            title: snapshot.val().title,
            description: snapshot.val().description
        };
    }

    static async loadAllFolders() {
        const snapshot = await this.db.ref('users/' + Auth.currentUser.uid + '/folders').once('value');
        const folders = []
        snapshot.forEach((childSnap) => {
            folders.push({
                id: childSnap.key,
                title: childSnap.val().title,
                description: childSnap.val().description
            });
        });
        return Promise.all(folders);
    }

    static async saveNote(folderID, content, date) {
        const newNoteRef = this.db.ref('users/' + Auth.currentUser.uid + '/folders/' + folderID + '/notes').push();
        newNoteRef.set({
            content: content,
            date: date
        });
        return newNoteRef.key;
    }

    static async loadNote(folderID, noteID) {
        const snapshot = await this.db.ref('users/' + Auth.currentUser.uid + '/folders/' + folderID + '/notes/' + noteID).once('value');
        return {
            id: snapshot.key,
            folderID: folderID,
            content: snapshot.val().content,
            date: snapshot.val().date
        };
    }

    static async loadAllNotes(folderID) {
        const snapshot = await this.db.ref('users/' + Auth.currentUser.uid + '/folders/' + folderID + '/notes').once('value');
        const notes = []
        snapshot.forEach((childSnap) => {
            notes.push({
                id: childSnap.key,
                content: childSnap.val().content,
                description: childSnap.val().date
            });
        });
        return Promise.all(notes);
    }

    static async updateNote(folderID, noteID, newContent) {
        const noteRef = this.db.ref('users/' + Auth.currentUser.uid + '/folders/' + folderID + '/notes/' + noteID);
        noteRef.update({
            content: newContent
        });
    }

    static init() {
        this.db = firebase.database();
    }
}
