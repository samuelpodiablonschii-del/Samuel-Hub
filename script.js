
const PASSWORD = "SamBinary";
const KEY = "sambinaryHub";
const $ = id => document.getElementById(id);

const defaults = () => ({
  coins: 0,
  played: 0,
  best: {},
  skins: ["mint"],
  skin: "mint",
  presets: Array.from({length: 10}, (_, i) => ({
    id: i + 1,
    name: `Empty slot ${i + 1}`,
    language: "Other",
    code: "",
    empty: true
  })),
  tasks: [],
  notes: ""
});

let state;
try {
  state = {...defaults(), ...JSON.parse(localStorage.getItem(KEY) || "{}")};
} catch {
  state = defaults();
}

let selected = "tap";
let run = {};
let editing = null;
let timerSeconds = 1500;
let timerInterval = null;
let toastTimeout;

const games = [
  {id:"tap", name:"Tap Sprint", icon:"⚡", desc:"Tap as many times as possible in 15 seconds."},
  {id:"reaction", name:"Reaction Test", icon:"🟢", desc:"Wait for green, then tap as quickly as possible."},
  {id:"memory", name:"Memory Match", icon:"🧠", desc:"Find all six matching pairs."},
  {id:"math", name:"Math Dash", icon:"∑", desc:"Solve as many maths questions as you can in 25 seconds."},
  {id:"sequence", name:"Sequence", icon:"🔷", desc:"Remember the sequence and repeat it."},
  {id:"higher", name:"Higher or Lower", icon:"🎯", desc:"Guess whether the next number is higher or lower."}
];

const skinOptions = [
  {id:"mint",name:"Neon Mint",color:"#70e1e8",price:0},
  {id:"violet",name:"Electric Violet",color:"#a59bff",price:40},
  {id:"rose",name:"Pixel Rose",color:"#ff9fc9",price:60},
  {id:"gold",name:"Solar Gold",color:"#f6d68b",price:85},
  {id:"lime",name:"Arcade Lime",color:"#b4f879",price:110},
  {id:"blue",name:"Ice Blue",color:"#8ebdff",price:135},
  {id:"orange",name:"Turbo Orange",color:"#ffad73",price:160},
  {id:"red",name:"Ruby Pulse",color:"#ff7c93",price:200}
];

function save() {
  try {
    localStorage.setItem(KEY, JSON.stringify(state));
  } catch {
    toast("Browser storage is unavailable");
  }
  updateCoins();
}

function updateCoins() {
  $("coins").textContent = state.coins;
  $("coins2").textContent = state.coins;
}

function toast(message) {
  const el = $("toast");
  el.textContent = message;
  el.classList.add("show");
  clearTimeout(toastTimeout);
  toastTimeout = setTimeout(() => el.classList.remove("show"), 2200);
}

function escapeHTML(value) {
  return String(value).replace(/[&<>"']/g, c => ({
    "&":"&amp;", "<":"&lt;", ">":"&gt;", '"':"&quot;", "'":"&#39;"
  })[c]);
}

function show(id) {
  ["login", "dashboard", "workspace"].forEach(name =>
    $(name).classList.toggle("hidden", name !== id)
  );
}

$("loginForm").addEventListener("submit", event => {
  event.preventDefault();
  if ($("password").value === PASSWORD) {
    $("loginError").textContent = "";
    $("password").value = "";
    show("dashboard");
    toast("Welcome to SamBinary Hub");
  } else {
    $("loginError").textContent = "Incorrect password.";
  }
});

$("logout").addEventListener("click", () => {
  show("login");
  toast("Workspace locked");
});

document.querySelectorAll("[data-open]").forEach(button => {
  button.addEventListener("click", () => openPage(button.dataset.open));
});

$("back").addEventListener("click", () => show("dashboard"));

function openPage(page) {
  show("workspace");
  document.querySelectorAll(".page").forEach(el => el.classList.add("hidden"));
  $(`${page}Page`).classList.remove("hidden");
  $("pageTitle").textContent = ({
    games:"Games", presets:"Code presets", tools:"School tools", planner:"Task planner"
  })[page];

  if (page === "games") renderGames();
  if (page === "presets") renderPresets();
  if (page === "tools") initTools();
  if (page === "planner") renderTasks();
  updateCoins();
}

document.querySelectorAll("[data-tab]").forEach(button => {
  button.addEventListener("click", () => {
    document.querySelectorAll("[data-tab]").forEach(b =>
      b.classList.toggle("active", b === button)
    );
    ["play", "shop", "stats"].forEach(tab =>
      $(`${tab}Tab`).classList.toggle("hidden", tab !== button.dataset.tab)
    );
    if (button.dataset.tab === "shop") renderShop();
    if (button.dataset.tab === "stats") renderStats();
  });
});

function stopGame() {
  clearTimeout(run.timeout);
  clearInterval(run.interval);
  run = {};
}

function renderGames() {
  $("gameList").innerHTML = games.map(game => `
    <button class="game-choice ${selected === game.id ? "active" : ""}" data-game="${game.id}">
      <span>${game.icon} ${game.name}</span>
      <small>Best: ${state.best[game.id] || 0}</small>
    </button>
  `).join("");

  document.querySelectorAll("[data-game]").forEach(button => {
    button.addEventListener("click", () => selectGame(button.dataset.game));
  });

  selectGame(selected, false);
}

function selectGame(id, stop = true) {
  if (stop) stopGame();
  selected = id;
  const game = games.find(g => g.id === id);

  $("gameTitle").textContent = game.name;
  $("gameCategory").textContent = game.icon + " ARCADE";
  $("gameDescription").textContent = game.desc;
  $("gameArea").textContent = "Press Start game when you're ready.";
  $("gameMessage").textContent = "";
  $("startGame").textContent = "Start game";

  document.querySelectorAll("[data-game]").forEach(button =>
    button.classList.toggle("active", button.dataset.game === id)
  );
}

$("startGame").addEventListener("click", startGame);
$("resetGame").addEventListener("click", () => selectGame(selected));

function startGame() {
  stopGame();
  state.played++;
  save();

  ({
    tap: startTap,
    reaction: startReaction,
    memory: startMemory,
    math: startMath,
    sequence: startSequence,
    higher: startHigher
  })[selected]();
}

function finish(score, message, reward = Math.max(3, Math.floor(score / 2))) {
  stopGame();
  state.best[selected] = Math.max(state.best[selected] || 0, score);
  state.coins += reward;
  save();
  $("gameMessage").textContent = `${message} · +${reward} coins`;
  $("gameTitle").textContent = games.find(g => g.id === selected).name;
  $("gameArea").insertAdjacentHTML("beforeend",
    `<p>Finished! Score: ${score}</p>`
  );
  $("startGame").textContent = "Play again";
  toast(`${message} · +${reward} coins`);
  renderGames();
}

function startTap() {
  let score = 0;
  let time = 15;
  $("gameArea").innerHTML = `
    <div><p id="tapTime">15 seconds</p>
    <button id="tapTarget" class="tap-target">TAP!</button>
    <h2 id="tapScore">0</h2></div>`;

  $("tapTarget").addEventListener("click", () => {
    score++;
    $("tapScore").textContent = score;
  });

  run.interval = setInterval(() => {
    time--;
    $("tapTime").textContent = `${time} seconds`;
    if (time <= 0) finish(score, `${score} taps`, Math.max(3, Math.floor(score / 3)));
  }, 1000);
}

function startReaction() {
  $("gameArea").innerHTML = `<button id="reactionBox" class="reaction-box">Tap to begin</button>`;
  const box = $("reactionBox");
  let phase = "start";
  let started = 0;

  box.addEventListener("click", () => {
    if (phase === "start" || phase === "false") {
      phase = "wait";
      box.classList.remove("go");
      box.textContent = "Wait for green...";
      run.timeout = setTimeout(() => {
        phase = "go";
        started = performance.now();
        box.classList.add("go");
        box.textContent = "TAP NOW!";
      }, 1300 + Math.random() * 2600);
    } else if (phase === "wait") {
      clearTimeout(run.timeout);
      phase = "false";
      box.textContent = "Too early! Tap to retry.";
    } else {
      const ms = Math.round(performance.now() - started);
      finish(Math.max(0, 1000 - ms), `${ms} ms reaction`, Math.max(5, Math.floor((1000 - ms) / 100)));
    }
  });
}

function startMemory() {
  const symbols = ["◆","●","▲","★","☂","✿"];
  const cards = [...symbols, ...symbols].sort(() => Math.random() - .5);
  let first = null, matches = 0, moves = 0, locked = false;

  $("gameArea").innerHTML = `
    <div><p>Find all six matching pairs</p>
    <div class="memory-grid">${cards.map((_, i) =>
      `<button data-card="${i}">?</button>`).join("")}</div>
    <p id="memoryMoves">Moves: 0</p></div>`;

  const buttons = [...document.querySelectorAll("[data-card]")];

  buttons.forEach(button => button.addEventListener("click", () => {
    const i = Number(button.dataset.card);
    if (locked || button.classList.contains("revealed")) return;

    button.textContent = cards[i];
    button.classList.add("revealed");

    if (first === null) {
      first = i;
      return;
    }

    moves++;
    $("memoryMoves").textContent = `Moves: ${moves}`;

    if (cards[first] === cards[i]) {
      matches++;
      first = null;
      if (matches === 6) finish(Math.max(1, 100 - moves), `Solved in ${moves} moves`, Math.max(8, 30 - moves));
    } else {
      locked = true;
      const previous = first;
      first = null;
      run.timeout = setTimeout(() => {
        buttons[previous].textContent = "?";
        button.textContent = "?";
        buttons[previous].classList.remove("revealed");
        button.classList.remove("revealed");
        locked = false;
      }, 650);
    }
  }));
}

function startMath() {
  let score = 0, total = 0, time = 25, answer = 0;

  $("gameArea").innerHTML = `
    <div><p id="mathTime">25 seconds</p>
    <h2 id="mathQuestion"></h2>
    <input id="mathAnswer" type="number" inputmode="numeric" placeholder="Your answer">
    <button id="mathSubmit" class="primary">Submit</button>
    <p id="mathScore">Score: 0</p></div>`;

  function ask() {
    const a = 2 + Math.floor(Math.random() * 18);
    const b = 2 + Math.floor(Math.random() * 18);
    const multiply = Math.random() < .5;
    answer = multiply ? a * b : a + b;
    $("mathQuestion").textContent = `${a} ${multiply ? "×" : "+"} ${b} = ?`;
    $("mathAnswer").value = "";
    $("mathAnswer").focus();
  }

  function submit() {
    if ($("mathAnswer").value === "") return;
    if (Number($("mathAnswer").value) === answer) score++;
    total++;
    $("mathScore").textContent = `Score: ${score}/${total}`;
    ask();
  }

  $("mathSubmit").addEventListener("click", submit);
  $("mathAnswer").addEventListener("keydown", event => {
    if (event.key === "Enter") submit();
  });

  ask();

  run.interval = setInterval(() => {
    time--;
    $("mathTime").textContent = `${time} seconds`;
    if (time <= 0) finish(score, `Math score ${score}/${total}`, Math.max(3, score * 3));
  }, 1000);
}

function startSequence() {
  let sequence = [], level = 1, index = 0, accepting = false;
  $("gameArea").innerHTML = `
    <div><p id="sequenceLabel">Watch the sequence</p>
    <div class="sequence-grid">${[0,1,2,3].map(i =>
      `<button data-sequence="${i}">${i + 1}</button>`).join("")}</div></div>`;

  const buttons = [...document.querySelectorAll("[data-sequence]")];

  async function play() {
    accepting = false;
    index = 0;
    sequence.push(Math.floor(Math.random() * 4));
    $("sequenceLabel").textContent = `Level ${level}: watch`;

    for (const n of sequence) {
      await new Promise(resolve => setTimeout(resolve, 300));
      buttons[n].classList.add("lit");
      await new Promise(resolve => setTimeout(resolve, 400));
      buttons[n].classList.remove("lit");
    }

    accepting = true;
    $("sequenceLabel").textContent = "Repeat the sequence";
  }

  buttons.forEach((button, i) => button.addEventListener("click", () => {
    if (!accepting) return;

    if (i !== sequence[index]) {
      finish(level - 1, `Reached level ${level}`, Math.max(3, level * 4));
      return;
    }

    index++;
    if (index === sequence.length) {
      level++;
      accepting = false;
      run.timeout = setTimeout(play, 500);
    }
  }));

  play();
}

function startHigher() {
  let current = 1 + Math.floor(Math.random() * 10);
  let score = 0, guesses = 0;

  $("gameArea").innerHTML = `
    <div><p>Will the next number be higher or lower?</p>
    <h2 id="higherNumber" class="number-display">${current}</h2>
    <div class="number-options">
      <button id="higherUp">↑ Higher</button>
      <button id="higherDown">↓ Lower</button>
    </div><p id="higherScore">Correct: 0</p></div>`;

  function guess(direction) {
    const next = 1 + Math.floor(Math.random() * 10);
    const actual = next > current ? "up" : next < current ? "down" : "same";

    if (actual === direction) score++;
    current = next;
    guesses++;

    $("higherNumber").textContent = current;
    $("higherScore").textContent = `Correct: ${score}/${guesses}`;

    if (guesses >= 10) finish(score, `${score}/10 correct`, Math.max(3, score * 2));
  }

  $("higherUp").addEventListener("click", () => guess("up"));
  $("higherDown").addEventListener("click", () => guess("down"));
}

function renderShop() {
  $("skinList").innerHTML = skinOptions.map(skin => {
    const owned = state.skins.includes(skin.id);
    const equipped = state.skin === skin.id;

    return `<div class="panel shop-skin">
      <span class="skin-orb" style="--skin:${skin.color}"></span>
      <h2>${skin.name}</h2>
      <p>${skin.price ? `◈ ${skin.price} coins` : "Starter skin"}</p>
      <button class="secondary skin-card-button" data-skin="${skin.id}">
        ${equipped ? "Equipped" : owned ? "Equip" : "Buy"}
      </button>
    </div>`;
  }).join("");

  document.querySelectorAll("[data-skin]").forEach(button => {
    button.addEventListener("click", () => {
      const skin = skinOptions.find(item => item.id === button.dataset.skin);

      if (state.skins.includes(skin.id)) {
        state.skin = skin.id;
      } else if (state.coins >= skin.price) {
        state.coins -= skin.price;
        state.skins.push(skin.id);
        state.skin = skin.id;
      } else {
        toast(`You need ${skin.price - state.coins} more coins`);
        return;
      }

      document.documentElement.style.setProperty("--accent", skin.color);
      save();
      renderShop();
      toast(`${skin.name} selected`);
    });
  });
}

function renderStats() {
  $("stats").textContent =
    `Coins: ${state.coins} · Games played: ${state.played} · Skins owned: ${state.skins.length} · Tasks completed: ${state.tasks.filter(t => t.done).length}`;

  $("records").innerHTML = games.map(game =>
    `<p>${game.icon} ${game.name}: <b>${state.best[game.id] || 0}</b></p>`
  ).join("");
}

$("presetSearch").addEventListener("input", renderPresets);

function renderPresets() {
  const query = $("presetSearch").value.toLowerCase();
  const filtered = state.presets.filter(p =>
    `${p.name} ${p.language} ${p.code}`.toLowerCase().includes(query)
  );

  $("presetList").innerHTML = filtered.map(p => `
    <div class="preset-item">
      <h3>${escapeHTML(p.name)}</h3>
      <small>${escapeHTML(p.language)}</small>
      <pre>${escapeHTML(p.code || "Empty slot — edit and paste your code.")}</pre>
      <div class="actions">
        <button class="primary" data-copy="${p.id}">Copy code</button>
        <button class="secondary" data-edit="${p.id}">Edit</button>
      </div>
    </div>
  `).join("") || "<p>No matching presets.</p>";

  document.querySelectorAll("[data-edit]").forEach(button => {
    button.addEventListener("click", () => editPreset(Number(button.dataset.edit)));
  });

  document.querySelectorAll("[data-copy]").forEach(button => {
    button.addEventListener("click", () => copyPreset(Number(button.dataset.copy)));
  });
}

function editPreset(id) {
  editing = state.presets.find(p => p.id === id);
  $("editorTitle").textContent = editing ? `Edit ${editing.name}` : "New preset";
  $("presetName").value = editing && !editing.empty ? editing.name : "";
  $("presetLanguage").value = editing ? editing.language : "C++";
  $("presetCode").value = editing && !editing.empty ? editing.code : "";
  $("editor").classList.remove("hidden");
  $("editor").scrollIntoView({behavior:"smooth", block:"start"});
}

$("savePreset").addEventListener("click", () => {
  const name = $("presetName").value.trim();
  if (!name) {
    toast("Enter a preset title first");
    return;
  }

  const data = {
    name,
    language: $("presetLanguage").value,
    code: $("presetCode").value,
    empty: false
  };

  if (editing) Object.assign(editing, data);
  else state.presets.push({id: Date.now(), ...data});

  save();
  renderPresets();
  $("editor").classList.add("hidden");
  toast("Preset saved");
});

$("cancelEdit").addEventListener("click", () => $("editor").classList.add("hidden"));

async function copyPreset(id) {
  const preset = state.presets.find(p => p.id === id);
  if (!preset || !preset.code) {
    toast("This preset is empty");
    return;
  }

  try {
    await navigator.clipboard.writeText(preset.code);
    toast("Code copied to clipboard");
  } catch {
    const area = document.createElement("textarea");
    area.value = preset.code;
    document.body.appendChild(area);
    area.select();
    const copied = document.execCommand("copy");
    area.remove();
    toast(copied ? "Code copied" : "Clipboard permission denied");
  }
}

function initTools() {
  $("scratchpad").value = state.notes || "";
  renderTimer();
}

$("scratchpad").addEventListener("input", () => {
  state.notes = $("scratchpad").value;
  save();
});

$("clearScratch").addEventListener("click", () => {
  $("scratchpad").value = "";
  state.notes = "";
  save();
  toast("Notes cleared");
});

document.querySelectorAll("[data-minutes]").forEach(button => {
  button.addEventListener("click", () => {
    stopTimer();
    timerSeconds = Number(button.dataset.minutes) * 60;
    document.querySelectorAll("[data-minutes]").forEach(b =>
      b.classList.toggle("active", b === button)
    );
    renderTimer();
  });
});

$("timerStart").addEventListener("click", () => {
  if (timerInterval) {
    stopTimer();
    return;
  }

  $("timerStart").textContent = "Pause";
  timerInterval = setInterval(() => {
    timerSeconds--;
    renderTimer();

    if (timerSeconds <= 0) {
      stopTimer();
      $("timerMessage").textContent = "Session complete. Nice work!";
      toast("Study session complete");
    }
  }, 1000);
});

$("timerReset").addEventListener("click", () => {
  stopTimer();
  const active = document.querySelector("[data-minutes].active");
  timerSeconds = Number(active?.dataset.minutes || 25) * 60;
  renderTimer();
});

function stopTimer() {
  clearInterval(timerInterval);
  timerInterval = null;
  $("timerStart").textContent = "Start";
}

function renderTimer() {
  const minutes = Math.floor(Math.max(0, timerSeconds) / 60);
  const seconds = Math.max(0, timerSeconds) % 60;
  $("timerDisplay").textContent =
    `${String(minutes).padStart(2, "0")}:${String(seconds).padStart(2, "0")}`;
}

$("taskForm").addEventListener("submit", event => {
  event.preventDefault();
  const title = $("taskInput").value.trim();
  if (!title) return;

  state.tasks.unshift({id:Date.now(), title, done:false});
  $("taskInput").value = "";
  save();
  renderTasks();
});

function renderTasks() {
  const completed = state.tasks.filter(t => t.done).length;
  $("taskProgress").textContent = `${completed} of ${state.tasks.length} tasks complete`;

  $("taskList").innerHTML = state.tasks.map(task => `
    <div class="task-row ${task.done ? "done" : ""}">
      <input type="checkbox" data-check="${task.id}" ${task.done ? "checked" : ""}>
      <span>${escapeHTML(task.title)}</span>
      <button data-delete="${task.id}" aria-label="Delete task">×</button>
    </div>
  `).join("") || "<p class='muted'>Your task list is empty.</p>";

  document.querySelectorAll("[data-check]").forEach(input => {
    input.addEventListener("change", () => {
      const task = state.tasks.find(t => t.id === Number(input.dataset.check));
      task.done = input.checked;
      save();
      renderTasks();
    });
  });

  document.querySelectorAll("[data-delete]").forEach(button => {
    button.addEventListener("click", () => {
      state.tasks = state.tasks.filter(t => t.id !== Number(button.dataset.delete));
      save();
      renderTasks();
    });
  });
}

const currentSkin = skinOptions.find(s => s.id === state.skin);
if (currentSkin) document.documentElement.style.setProperty("--accent", currentSkin.color);

updateCoins();
