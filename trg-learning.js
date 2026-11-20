(() => {
  "use strict";

  const STORAGE_KEY = "samuelhub_trg_completed_lessons";
  const $ = (id) => document.getElementById(id);

  const courses = [
    {
      level: 1, title: "First steps", subtitle: "Meet the language",
      description: "Start with tiny programs and learn how TRG stores and changes information.",
      lessons: [
        {
          id: "l1-first-program", title: "Your first TRG program", time: "5 min",
          intro: "A TRG program is a sequence of commands. The simplest command is say: it writes a message to the output console.",
          sections: [
            ["The say command", "Write say followed by the text you want to display. Put text in double quotes when it contains spaces."],
            ["Run it", "Open the TRG IDE, press Run TRG, and look in Output / console. TRG runs through the real WebAssembly interpreter used by this website."]
          ],
          code: 'say "Hello from TRG"\nsay "I just wrote my first program!"',
          challenge: "Change the two messages to introduce yourself. Add a third say command that tells the reader what you want to build.",
          tip: "TRG commands go on separate lines. You do not need semicolons."
        },
        {
          id: "l1-variables", title: "Words, numbers and switches", time: "8 min",
          intro: "Variables keep information so your program can use it later. TRG has named types such as word, num and toggle.",
          sections: [
            ["Declare a variable", "Start with its type, then its name. Use = to give it an initial value. A word stores text, num stores a number, and toggle stores true or false."],
            ["Use the value", "Write the variable name in a command. TRG substitutes its current value when the program runs."]
          ],
          code: 'word player = "Nova"\nnum score = 12\ntoggle ready = true\nsay "Player: " player\nsay "Score: " score\nsay "Ready: " ready',
          challenge: "Create a word called game, a num called lives and a toggle called playing. Give each one a value, then print all three.",
          tip: "Variable names can use letters, numbers and underscores, but should start with a letter or underscore."
        },
        {
          id: "l1-maths", title: "Calculations and changing values", time: "8 min",
          intro: "TRG can evaluate numeric expressions. Use set to replace a variable's value, or increase and decrease to adjust a number.",
          sections: [
            ["Math expressions", "Use +, -, *, /, %, parentheses and ^ for powers in numeric expressions."],
            ["Change a number", "increase score adds one by default. Add a second number to choose the amount. decrease works the same way in the opposite direction."]
          ],
          code: 'num score = 10\nincrease score 5\nsay "Bonus applied: " score\nset score = (score + 5) * 2\nsay "Final score: " score',
          challenge: "Start with 20 coins, add 7, subtract 3, then double the amount with set. Print the final total.",
          tip: "Use set score = score + 1 when you want to calculate and assign in one step."
        }
      ]
    },
    {
      level: 2, title: "Input and logic", subtitle: "Make programs respond",
      description: "Ask questions, make decisions and repeat work with conditions.",
      lessons: [
        {
          id: "l2-input", title: "Ask the user for input", time: "8 min",
          intro: "The ask command displays a prompt and stores the answer in a variable. In this browser IDE, answers are supplied in the Input for ask panel before you run.",
          sections: [
            ["Ask for a word", "Write ask, a quoted prompt, and the variable name that should receive the answer."],
            ["Try different answers", "Put one answer per line in the input panel. If a program asks several questions, TRG reads the lines in order."]
          ],
          code: 'word name\nask "What is your name?" name\nsay "Welcome to TRG, " name',
          input: "Alex",
          challenge: "Ask for a favourite game and store it in a variable named favourite. Print a sentence that includes the answer.",
          tip: "Declare a num before asking for a number, and a toggle before asking for true or false."
        },
        {
          id: "l2-decisions", title: "Make decisions with if and else", time: "10 min",
          intro: "An if block runs only when its condition is true. An else block runs when the condition is false.",
          sections: [
            ["Conditions", "Compare values with ==, !=, <, >, <= and >=. You can compare numbers and words."],
            ["TRG blocks", "Blocks start with an opening parenthesis on the command line and end with a line containing a closing parenthesis. Curly braces are not used."]
          ],
          code: 'num score = 42\nif score >= 40 (\n    say "Level complete!"\n)\nelse (\n    say "Keep practising."\n)',
          challenge: "Create a variable named temperature. Print 'Warm' when it is at least 20, otherwise print 'Cool'.",
          tip: "Keep the opening and closing parentheses on their own lines so blocks are easy to read."
        },
        {
          id: "l2-loops", title: "Repeat code with loop", time: "10 min",
          intro: "A loop repeats its block while a condition remains true. Make sure something inside the block changes the condition, or the loop may never end.",
          sections: [
            ["A counted loop", "Create a numeric counter, test it in the loop condition, and increase it inside the block."],
            ["Control the loop", "break exits the current loop. continue moves to its next iteration."]
          ],
          code: 'num counter = 1\nloop counter <= 5 (\n    say "Count: " counter\n    increase counter\n)\nsay "Finished!"',
          challenge: "Write a loop that prints the even numbers 2, 4, 6, 8 and 10.",
          tip: "The browser runtime has a loop guard, but your own programs should still use conditions that eventually become false."
        }
      ]
    },
    {
      level: 3, title: "Data and reusable code", subtitle: "Build bigger programs",
      description: "Work with lists and text, then structure your program with functions and records.",
      lessons: [
        {
          id: "l3-random", title: "Random numbers", time: "6 min",
          intro: "random chooses a number between a minimum and maximum and stores it in a result variable.",
          sections: [
            ["The random command", "Use random MIN MAX RESULT. The result variable is created as a numeric value."],
            ["Use randomness", "Random values are useful for dice, simple games, choices and test data."]
          ],
          code: 'random 1 6 roll\nsay "You rolled: " roll\nif roll == 6 (\n    say "Maximum roll!"\n)',
          challenge: "Generate a random number from 1 to 100 and print it. Add a message for numbers above 50.",
          tip: "The minimum must be less than or equal to the maximum."
        },
        {
          id: "l3-lists", title: "Store collections in lists", time: "10 min",
          intro: "A list stores several values under one variable name. You can add, retrieve and remove items.",
          sections: [
            ["Create and add items", "Use list NAME = VALUE, VALUE to create a list, then push LIST VALUE to add an item."],
            ["Read list items", "get LIST INDEX VARIABLE copies an item into a variable. List indexes start at zero. length LIST RESULT stores its item count."]
          ],
          code: 'list scores = 10, 20, 30\npush scores 40\nget scores 0 first_score\nlength scores item_count\nsay "First score: " first_score\nsay "Items: " item_count',
          challenge: "Create a list of three favourite games. Push a fourth game, get the item at index 2 and print it.",
          tip: "Use remove LIST INDEX to remove an item at a chosen zero-based index."
        },
        {
          id: "l3-text", title: "Transform and inspect text", time: "9 min",
          intro: "TRG includes commands for changing and inspecting words without writing a separate function each time.",
          sections: [
            ["Change case", "upper VALUE RESULT converts text to uppercase. lower VALUE RESULT converts it to lowercase."],
            ["Search and measure", "length VALUE RESULT returns the character count. contains VALUE SEARCH RESULT returns true or false. replace VALUE SEARCH REPLACEMENT RESULT creates a changed copy."]
          ],
          code: 'word username = "trg learner"\nupper username title\nlength username size\ncontains username "learner" found\nsay title\nsay "Characters: " size\nsay "Contains learner: " found',
          challenge: "Create a phrase, convert it to lowercase, check whether it contains a word of your choice and print the result.",
          tip: "These commands create a result variable; they do not overwrite the original text."
        }
      ]
    },
    {
      level: 4, title: "Program architecture", subtitle: "Functions, records and files",
      description: "Make code easier to reuse and organise projects into meaningful data and files.",
      lessons: [
        {
          id: "l4-functions", title: "Create your own commands", time: "12 min",
          intro: "A function lets you name a group of commands and run them whenever you need. In TRG, define introduces the function and the next block contains its instructions.",
          sections: [
            ["Define a function", "Write define, the function name, then its parameter names. Start the body with an opening parenthesis."],
            ["Call it", "Write the function name followed by values for its parameters. TRG runs the function body with those values."]
          ],
          code: 'define greet name (\n    say "Welcome, " name\n)\ngreet "Developer"\ngreet "TRG learner"',
          challenge: "Create a function named show_score with one parameter. Make it print a message containing the score, then call it twice with different values.",
          tip: "Function parameters are temporary values used while that function runs."
        },
        {
          id: "l4-records", title: "Model data with records", time: "12 min",
          intro: "A record groups related fields into one named type. Define it once, then create instances whenever you need one.",
          sections: [
            ["Define the record", "Use record NAME and list its fields inside a block. Fields can be word, num, toggle or color."],
            ["Create an instance", "Use new TYPE instanceName. Access fields through instanceName.fieldName, then update them with set."]
          ],
          code: 'record Player (\n    word name = "Nova"\n    num score = 0\n)\nnew Player hero\nset hero.score = 25\nsay hero.name\nsay hero.score',
          challenge: "Design a record called Game with a title and a level number. Create an instance, change the level and print both fields.",
          tip: "Records help keep related values together instead of using unrelated variable names."
        },
        {
          id: "l4-files", title: "Read and write project files", time: "12 min",
          intro: "TRG can work with project files when the files library is enabled. In the browser IDE, imported files are placed in the project workspace.",
          sections: [
            ["Enable file commands", "Add add (files) near the top of your program to enable file operations."],
            ["Read and write", "write_file writes a line, append_file adds to an existing file, read_file loads a file into a word variable and file_exists checks whether it exists."]
          ],
          code: 'add (files)\nwrite_file "notes.txt" "TRG save data"\nread_file "notes.txt" saved_text\nsay saved_text\nfile_exists "notes.txt" exists\nsay "File exists: " exists',
          challenge: "Write a file called score.txt, read it into a variable and display the saved value. Import files into the IDE if your project needs them.",
          tip: "TRG project paths are kept inside the project folder; path traversal outside it is rejected."
        }
      ]
    },
    {
      level: 5, title: "Interactive building", subtitle: "UI, animation and web",
      description: "Use TRG's visual tools to build interfaces and pages that respond to people.",
      lessons: [
        {
          id: "l5-ui", title: "Build a visual interface", time: "14 min",
          intro: "The UI library lets TRG create text, shapes, images and buttons. In the browser IDE, the visual result appears in the TRG UI preview.",
          sections: [
            ["Enable the UI library", "Start with add (ui). A text command positions a label using x and y coordinates, with size and colour properties."],
            ["Add shapes", "block creates a visual block. Give it a name, position, dimensions and a colour. Shape can be square, circle, triangle, pentagon, hexagon or star."]
          ],
          code: 'add (ui)\ntext title "My first interface" x 50 y 15 size 6 color "#FFFFFF" align center bold true\nblock name orb x 44 y 35 w 12 h 12 color "#A394FF" shape circle',
          challenge: "Create a title and two differently coloured shapes. Give the shapes different positions and sizes.",
          tip: "The preview uses percentage-based coordinates, so your UI scales with the preview area."
        },
        {
          id: "l5-buttons", title: "Make buttons interactive", time: "15 min",
          intro: "A TRG button has a label, a position, a size and a block of commands. Those commands run when the button is clicked.",
          sections: [
            ["Create a button", "Use button NAME LABEL x X y Y w WIDTH h HEIGHT followed by a block."],
            ["Update the interface", "Use a variable to track state. Update a named text object's value with itsName text VALUE from inside the button block."]
          ],
          code: 'add (ui)\nnum clicks = 0\ntext counter "Clicks: 0" x 50 y 28 size 5 color "#FFFFFF" align center\nbutton add_one "Add one" x 30 y 50 w 40 h 14 (\n    increase clicks 1\n    counter text clicks\n)',
          challenge: "Build a counter with an Add button and a Reset button. Both should update the text shown in the preview.",
          tip: "Button blocks use the same TRG commands as the rest of your program."
        },
        {
          id: "l5-animation", title: "Animate visual objects", time: "12 min",
          intro: "Animation commands move named UI objects over a chosen duration. The browser preview advances animations using the TRG runtime tick.",
          sections: [
            ["Animate a block", "Use animate NAME x DX y DY fps RATE time SECONDS after creating the block."],
            ["Choose your motion", "DX and DY are the total movement. A positive DX moves right; a positive DY moves down."]
          ],
          code: 'add (ui)\nblock name dot x 10 y 45 w 8 h 8 color "#57D6A1" shape circle\nanimate dot x 70 y 0 fps 60 time 4s',
          challenge: "Create two shapes and animate them in different directions. Try different durations and rates.",
          tip: "The animation time must be greater than zero and the frame rate must also be positive."
        }
      ]
    },
    {
      level: 6, title: "Advanced projects", subtitle: "Build complete experiences",
      description: "Combine libraries and concepts to create larger projects with structured data and interaction.",
      lessons: [
        {
          id: "l6-web", title: "Generate a web page with TRG", time: "15 min",
          intro: "The TRG Web library builds a web page from TRG commands. The IDE displays the generated page in its Web preview.",
          sections: [
            ["Start a page", "Enable add (web), then use web_page TITLE to create the page structure."],
            ["Add content", "Use web_heading, web_text, web_button, web_color and web_image to add page elements. Finish with web_show."]
          ],
          code: 'add (web)\nweb_page "My TRG Page"\nweb_heading "Built with TRG"\nweb_text "This page was generated by a TRG program."\nweb_button "Say hello" "Hello from the page!"\nweb_color "#171923"\nweb_show',
          challenge: "Create a mini profile page with a heading, two paragraphs, a colour of your choice and a button.",
          tip: "The preview is sandboxed in the browser. This WebAssembly port does not provide unrestricted network access."
        },
        {
          id: "l6-json", title: "Work with structured JSON", time: "15 min",
          intro: "The JSON library can turn variables created inside a named group into a JSON object. This is useful for save data, settings and project data.",
          sections: [
            ["Enable JSON", "Use add (json) before JSON commands."],
            ["Create a data group", "Variables introduced in a named group are associated with that group. Use json GROUP clean RESULT to create a formatted JSON string."]
          ],
          code: 'add (json)\ngroup player (\n    word name = "Nova"\n    num score = 250\n    toggle unlocked = true\n)\njson player clean saved_json\nsay saved_json',
          challenge: "Create a settings group with a username, sound toggle and difficulty value. Convert it to clean JSON and display it.",
          tip: "Use read_file and json read together when you want to load JSON stored in a project file."
        },
        {
          id: "l6-capstone", title: "Capstone: build a mini game", time: "25 min",
          intro: "This final lesson combines variables, conditions, UI text and button events. You will build a click-to-score game with a win condition.",
          sections: [
            ["Plan the state", "A numeric score records progress. The title and score text objects show the current state."],
            ["Add interaction", "The button increases the score, updates the score label and checks whether the target has been reached."],
            ["Extend the project", "Add a reset button, a timer, more targets or a second screen. Keep each part of the program small and test it as you go."]
          ],
          code: 'add (ui)\nnum score = 0\ntext title "Reach 10 points" x 50 y 15 size 6 color "#FFFFFF" align center\ntext score_label "0 / 10" x 50 y 32 size 5 color "#57D6A1" align center\nbutton gain "Score +1" x 30 y 48 w 40 h 13 (\n    increase score 1\n    score_label text score\n    if score >= 10 (\n        title text "You win!"\n    )\n)\nbutton reset "Reset" x 30 y 66 w 40 h 11 color "#55516F" (\n    set score = 0\n    score_label text score\n    title text "Reach 10 points"\n)',
          challenge: "Finish the game, then add your own rule: perhaps a bonus every five points, a second button or a timer. Make the interface clearly show the game's state.",
          tip: "You have reached the advanced track. Use the IDE to experiment, then save the final project as a .trg file."
        }
      ]
    }
  ];

  const lessons = courses.flatMap(course => course.lessons.map(lesson => ({
    ...lesson, level: course.level, levelTitle: course.title, levelSubtitle: course.subtitle
  })));
  let activeLessonId = null;

  function readCompleted() {
    try {
      const value = JSON.parse(localStorage.getItem(STORAGE_KEY) || "[]");
      return new Set(Array.isArray(value) ? value : []);
    } catch (_) { return new Set(); }
  }

  function writeCompleted(set) {
    localStorage.setItem(STORAGE_KEY, JSON.stringify(Array.from(set)));
  }

  function create(tag, className, text) {
    const node = document.createElement(tag);
    if (className) node.className = className;
    if (text !== undefined) node.textContent = text;
    return node;
  }

  function renderProgress() {
    const completed = readCompleted();
    const count = completed.size;
    const percent = Math.round(count / lessons.length * 100);
    if ($("learning-progress-value")) $("learning-progress-value").textContent = percent + "%";
    if ($("learning-progress-fill")) $("learning-progress-fill").style.width = percent + "%";
    if ($("learning-progress-count")) $("learning-progress-count").textContent = count + " of " + lessons.length + " lessons completed";
  }

  function renderOverview() {
    const holder = $("learning-level-grid");
    if (!holder) return;
    const completed = readCompleted();
    holder.replaceChildren();
    courses.forEach(course => {
      const done = course.lessons.filter(lesson => completed.has(lesson.id)).length;
      const card = create("article", "learning-level-card");
      const top = create("div", "learning-level-card-top");
      top.append(create("span", "learning-level-number", "LEVEL " + course.level));
      top.append(create("span", "learning-level-progress", done + " / " + course.lessons.length + " complete"));
      card.append(top);
      card.append(create("h2", "", course.title));
      card.append(create("p", "learning-level-subtitle", course.subtitle));
      card.append(create("p", "learning-level-description", course.description));
      const bar = create("div", "learning-level-track");
      const fill = create("i");
      fill.style.width = Math.round(done / course.lessons.length * 100) + "%";
      bar.append(fill);
      card.append(bar);
      const list = create("div", "learning-lesson-list");
      course.lessons.forEach((lesson, index) => {
        const row = create("button", "learning-lesson-row" + (completed.has(lesson.id) ? " is-complete" : ""));
        row.type = "button";
        const status = create("span", "learning-lesson-status", completed.has(lesson.id) ? "✓" : String(index + 1).padStart(2, "0"));
        const body = create("span", "learning-lesson-row-body");
        body.append(create("strong", "", lesson.title));
        body.append(create("small", "", lesson.time + " · " + lesson.intro));
        row.append(status, body, create("span", "learning-lesson-arrow", "↗"));
        row.addEventListener("click", () => openLesson(lesson.id));
        list.append(row);
      });
      card.append(list);
      const firstIncomplete = course.lessons.find(lesson => !completed.has(lesson.id)) || course.lessons[course.lessons.length - 1];
      const start = create("button", "learning-level-start", done === course.lessons.length ? "Review level ↗" : "Continue level ↗");
      start.type = "button";
      start.addEventListener("click", () => openLesson(firstIncomplete.id));
      card.append(start);
      holder.append(card);
    });
    renderProgress();
  }

  function openLesson(id) {
    const index = lessons.findIndex(lesson => lesson.id === id);
    if (index < 0) return;
    activeLessonId = id;
    const lesson = lessons[index];
    const completed = readCompleted();
    $("learning-overview").classList.add("hidden");
    $("learning-lesson").classList.remove("hidden");
    $("lesson-level-chip").textContent = "LEVEL " + lesson.level;
    $("lesson-position").textContent = "Lesson " + (index + 1) + " of " + lessons.length;
    $("lesson-kicker").textContent = "LEVEL " + lesson.level + " · " + lesson.levelSubtitle.toUpperCase();
    $("lesson-title").textContent = lesson.title;
    $("lesson-description").textContent = lesson.intro;
    $("lesson-challenge").textContent = lesson.challenge;
    $("lesson-code-example").textContent = lesson.code;
    $("lesson-input-note").textContent = lesson.input
      ? "Input panel example: " + lesson.input + ". Copy this into Input for ask before running."
      : lesson.tip;
    const body = $("lesson-body");
    body.replaceChildren();
    lesson.sections.forEach(section => {
      const block = create("section", "lesson-topic");
      block.append(create("h3", "", section[0]));
      block.append(create("p", "", section[1]));
      body.append(block);
    });
    if (lesson.tip) body.append(create("p", "lesson-tip", "TIP  " + lesson.tip));
    $("previous-lesson").disabled = index === 0;
    $("next-lesson").disabled = index === lessons.length - 1;
    $("complete-lesson").textContent = completed.has(id) ? "Completed ✓ · Continue →" : "Mark complete & continue →";
    $("lesson-completion-label").textContent = completed.has(id) ? "Completed" : "Not completed yet";
    $("learning-lesson").scrollIntoView({ behavior: "smooth", block: "start" });
  }

  function showOverview() {
    $("learning-lesson").classList.add("hidden");
    $("learning-overview").classList.remove("hidden");
    activeLessonId = null;
    renderOverview();
  }

  function loadExampleInIde() {
    const lesson = lessons.find(item => item.id === activeLessonId);
    if (!lesson) return;
    const editor = $("cpp-code");
    if (!editor) return;
    if (editor.value.trim() && editor.value.trim() !== lesson.code &&
        !window.confirm("Replace the code in your current TRG draft with this lesson example?")) return;
    if (typeof window.navigate === "function") window.navigate("editor");
    editor.value = lesson.code;
    if ($("code-input")) $("code-input").value = lesson.input || "";
    editor.dispatchEvent(new Event("input", { bubbles: true }));
    if (typeof window.saveDraft === "function") window.saveDraft();
    if (typeof window.updateTRGLineNumbers === "function") window.updateTRGLineNumbers();
    if ($("compiler-status")) $("compiler-status").textContent = "Lesson example loaded · press Run TRG";
    editor.focus();
  }

  async function copyExample() {
    const lesson = lessons.find(item => item.id === activeLessonId);
    if (!lesson) return;
    try {
      await navigator.clipboard.writeText(lesson.code);
      if (typeof window.toast === "function") window.toast("Lesson code copied.");
      else $("copy-lesson-code").textContent = "Copied!";
    } catch (_) {
      const temporary = document.createElement("textarea");
      temporary.value = lesson.code;
      document.body.append(temporary);
      temporary.select();
      try { document.execCommand("copy"); } catch (_) { }
      temporary.remove();
      $("copy-lesson-code").textContent = "Copied!";
    }
  }

  function goToLesson(delta, completeCurrent) {
    const index = lessons.findIndex(lesson => lesson.id === activeLessonId);
    if (index < 0) return;
    if (completeCurrent) {
      const completed = readCompleted();
      completed.add(activeLessonId);
      writeCompleted(completed);
    }
    const nextIndex = index + delta;
    if (nextIndex >= 0 && nextIndex < lessons.length) openLesson(lessons[nextIndex].id);
    else showOverview();
    renderProgress();
  }

  function render() {
    renderOverview();
    if (activeLessonId) openLesson(activeLessonId);
  }

  function setup() {
    document.querySelectorAll('[data-page="learn"],[data-goto="learn"]').forEach(button => {
      button.addEventListener("click", () => render());
    });
    $("back-to-lessons")?.addEventListener("click", showOverview);
    $("copy-lesson-code")?.addEventListener("click", copyExample);
    $("use-lesson-code")?.addEventListener("click", loadExampleInIde);
    $("previous-lesson")?.addEventListener("click", () => goToLesson(-1, false));
    $("next-lesson")?.addEventListener("click", () => goToLesson(1, false));
    $("complete-lesson")?.addEventListener("click", () => goToLesson(1, true));
    render();
  }

  window.TRGLearning = Object.freeze({
    render,
    openLesson,
    courses: courses.map(course => ({ level: course.level, title: course.title, lessons: course.lessons.length })),
    lessonCount: lessons.length
  });

  if (document.readyState === "loading") document.addEventListener("DOMContentLoaded", setup);
  else setup();
})();
