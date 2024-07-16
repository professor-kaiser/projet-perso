const close = document.getElementById("close-block")
const open = document.getElementById("open-block")
const btnBlock = document.getElementById("btn-block")
const blur = document.getElementById("blur")
const add = document.querySelector("#add")
const status = document.getElementById("status")
const allow = document.querySelectorAll(".allowButton")
const switchPassword = document.getElementById("lock-status")
const inputPassword = document.querySelector("#set-password")

function hello() {
	const inputSearch = document.getElementById("search")
	const sitesBlocked = document.querySelectorAll(".url-blocked")

	sitesBlocked.forEach(td => {
		const site = td.children.url

		if(!site.value.toUpperCase().includes(inputSearch.value.toUpperCase().trim())) {
			td.parentElement.style.display = "none"
		}
		else {
			td.parentElement.style.display = ""
		}
	})
}

function setStatus() {
	const form = document.getElementById("form-allow")

	if(status.checked) {
		allow.forEach(btn => {
			btn.setAttribute("disabled", "")
		})
	}
	else {
		allow.forEach(btn => {
			btn.removeAttribute("disabled")
			btn.addEventListener("click", () => {
				form.innerHTML = ""
				const children = btn.parentElement.children
				const div = document.getElementById("div-allow")
				const btnContainer = document.createElement("div")
				const btnYes = document.createElement("button")

				const cancelContainer = document.createElement("div")
				cancelContainer.classList.add("d-flex")
				cancelContainer.classList.add("justify-content-end")

				const cancelBtn = document.createElement("img")
				cancelBtn.setAttribute("src", "src/close.png")
				cancelBtn.setAttribute("width", "20")
				cancelBtn.setAttribute("height", "20")
				cancelBtn.classList.add("opacity-50")
				cancelBtn.classList.add("mb-3")
				cancelBtn.addEventListener("mouseover", () => {
					cancelBtn.style.cursor = "pointer"
				})
				cancelBtn.addEventListener("click", () => {
					blur.style.display = "none"
					div.style.display = "none"
					form.innerHTML = ""
				})

				btnContainer.classList.add("d-flex")
				btnContainer.classList.add("justify-content-around")

				btnYes.classList.add("btn")
				btnYes.classList.add("btn-danger")
				btnYes.classList.add("col-md-12")
				btnYes.classList.add("col-12")
				btnYes.classList.add("mt-4")
				btnYes.setAttribute("type", "submit")
				btnYes.textContent = "Unlock"

				btnContainer.appendChild(btnYes)

				cancelContainer.appendChild(cancelBtn)
				form.appendChild(cancelContainer)

				if(children.password.value === "") {
					const warn = document.createElement("h4")

					warn.classList.add("text-center")
					warn.classList.add("my-5")
					warn.textContent = "Are you sure to allow " + children.url.value + " ?"

					form.appendChild(warn)
					form.appendChild(children.url.cloneNode())
					form.appendChild(btnContainer)

					blur.style.display = "block"
					div.style.display = "block"
				}
				else {
					const head = document.createElement("h4")
					head.classList.add("text-center")
					head.textContent = "This site is locked"

					const line = document.createElement("line")
					line.classList.add("border-top")
					line.classList.add("col-md-12")
					line.classList.add("col-12")
					line.classList.add("mb-5")
					line.classList.add("row")

					const label = document.createElement("label")
					label.classList.add("form-label")
					label.setAttribute("for", "enter-password")
					label.textContent = "Enter password for: " + children.url.value

					const enterPassword = document.createElement("input")
					enterPassword.classList.add("form-control")
					enterPassword.setAttribute("type", "password")
					enterPassword.setAttribute("id", "enter-password")
					enterPassword.setAttribute("placeholder", "password")
					enterPassword.setAttribute("name", "password")

					btnContainer.classList.add("mt-4")

					form.appendChild(head)
					form.appendChild(line)
					form.appendChild(label)
					form.appendChild(enterPassword)
					form.appendChild(children.url.cloneNode())
					form.appendChild(btnContainer)

					blur.style.display = "block"
					div.style.display = "block"
				}
			})
		})
	}
}

function setSwitch() {
	if(switchPassword.checked) {
		inputPassword.style.display = "block"
		inputPassword.setAttribute("required", "")
	}
	else {
		inputPassword.style.display = "none"
		inputPassword.removeAttribute("required")
	}
}

setStatus()
setSwitch()

close.addEventListener("click", () => {
	blur.style.display = "none"
	add.style.display = "none"
})

open.addEventListener("click", () => {
	blur.style.display = "block"
	add.style.display = "block"
})

btnBlock.addEventListener("submit", () => {
	document.getElementById("loading-block").style.display = "block"
})