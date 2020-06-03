const profileModal = document.getElementById('profile-modal');
const activateButton = document.getElementById('button');

handleModalWindow(profileModal, activateButton);

const editButton = document.getElementById('edit-button');
const editForm = document.getElementById('edit-form');
const saveChangesButton = document.getElementById('save-changes-button');

editButton.addEventListener('click', () => {
    editForm.style.display = "flex";
    editButton.style.display = "none";
    saveChangesButton.style.display = "inline";
});

saveChangesButton.addEventListener('click', () => {
    editForm.style.display = "none";
    saveChangesButton.style.display = "none";
    editButton.style.display = "inline";
});