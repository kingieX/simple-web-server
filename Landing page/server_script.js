// side bars
var menu = document.querySelector('.menu');
var menuBtn = document.querySelector('.menu button');
menuBtn.addEventListener('click', () => {
    menu.classList.toggle('opened')
});

// Readmore on Overview
const moreContent = document.getElementById('more');
const readMoreLink = document.getElementById('read-more')

readMoreLink.addEventListener("click", (e) => {
  e.preventDefault();

  if (moreContent.style.display === "none") {
    moreContent.style.display = "block";
    readMoreLink.textContent = "Read less";
  } else {
    moreContent.style.display = "none";
    readMoreLink.textContent = "Read more"
  }
});

// Readmore on About
const moreContent1 = document.getElementById('more-1');
const readMoreLink1 = document.getElementById('read-more-1')

readMoreLink1.addEventListener("click", (e) => {
  e.preventDefault();

  if (moreContent1.style.display === "none") {
    moreContent1.style.display = "block";
    readMoreLink1.textContent = "Read less";
  } else {
    moreContent1.style.display = "none";
    readMoreLink1.textContent = "Read more"
  }
});

// show more message for contributors 1
const moreContents = document.querySelector('.more-2');
const readMoreLinks = document.querySelector('.read-more-2')

readMoreLinks.addEventListener("click", (e) => {
  e.preventDefault();

  if (moreContents.style.display === "none") {
    moreContents.style.display = "block";
    readMoreLinks.textContent = "Read less";
  } else {
    moreContents.style.display = "none";
    readMoreLinks.textContent = "Read more"
  }
});

// show more message for contributors 2
const moreContent2 = document.querySelector('.more-3');
const readMoreLink2 = document.querySelector('.read-more-3')

readMoreLink2.addEventListener("click", (e) => {
  e.preventDefault();

  if (moreContent2.style.display === "none") {
    moreContent2.style.display = "block";
    readMoreLink2.textContent = "Read less";
  } else {
    moreContent2.style.display = "none";
    readMoreLink2.textContent = "Read more"
  }
});

// show more message for contributors 3
const moreContent3 = document.querySelector('.more-4');
const readMoreLink3 = document.querySelector('.read-more-4')

readMoreLink3.addEventListener("click", (e) => {
  e.preventDefault();

  if (moreContent3.style.display === "none") {
    moreContent3.style.display = "block";
    readMoreLink3.textContent = "Read less";
  } else {
    moreContent3.style.display = "none";
    readMoreLink3.textContent = "Read more"
  }
});

// slides edits
var swiper = new Swiper(".slide-content", {
    slidesPerView: 1,
    spaceBetween: 30,
    loop: true,
    pagination: {
      el: ".swiper-pagination",
      clickable: true,
    },
    navigation: {
      nextEl: ".swiper-button-next",
      prevEl: ".swiper-button-prev",
    },
  });