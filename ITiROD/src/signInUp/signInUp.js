const signInForm = document.getElementById('sign-in-form');
const signUpForm = document.getElementById('sign-up-form');
const showSignIn = document.getElementById('show-sign-in');
const showSignUp = document.getElementById('show-sign-up');

showSignIn.addEventListener('click', () => {
    signInForm.style.display = "block";
    signUpForm.style.display = "none";
});

showSignUp.addEventListener('click', () => {
    signUpForm.style.display = "block";
    signInForm.style.display = "none";
});

