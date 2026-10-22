const $ = (id) => document.getElementById(id);
const DB = {
  read(key, fallback) { try { const v = localStorage.getItem("samuelhub_" + key); return v === null ? fallback : JSON.parse(v); } catch { return fallback; } },
  write(key, value) { localStorage.setItem("samuelhub_" + key, JSON.stringify(value)); }
};
let toastTimer;
function toast(message) { const el=$("toast"); el.textContent=message; el.classList.add("show"); clearTimeout(toastTimer); toastTimer=setTimeout(()=>el.classList.remove("show"),2200); }

const ACCESS_CODE = "328490";
const EMERGENCY_CODES = new Set(["0000", "....0000"]);
const SESSION_KEY = "samuelhub_session_end";
let sessionInterval;
function beginSession() { const end=Date.now()+30*60*1000; sessionStorage.setItem(SESSION_KEY,String(end)); showApp(); }
function showApp() { $("login-screen").classList.add("hidden"); $("app").classList.remove("hidden"); setupApp(); startSessionClock(); }
function startSessionClock() {
  clearInterval(sessionInterval);
  const update=()=>{ const end=Number(sessionStorage.getItem(SESSION_KEY)||0); const left=end-Date.now(); if(left<=0){clearInterval(sessionInterval);sessionStorage.removeItem(SESSION_KEY);$("app").classList.add("hidden");$("login-screen").classList.remove("hidden");$("password").value="";$("login-error").textContent="Session expired. Enter the access code again.";return;} const m=Math.floor(left/60000),s=Math.floor((left%60000)/1000);$("session-countdown").textContent=`${String(m).padStart(2,"0")}:${String(s).padStart(2,"0")} remaining`;};
  update(); sessionInterval=setInterval(update,1000);
}
$("login-form").addEventListener("submit",e=>{e.preventDefault();if($("password").value===ACCESS_CODE||EMERGENCY_CODES.has($("password").value)){$("login-error").textContent="";beginSession();}else{$("login-error").textContent="That code doesn't match. Try again.";$("password").select();}});
$("show-password").addEventListener("click",()=>{const p=$("password");p.type=p.type==="password"?"text":"password";$("show-password").textContent=p.type==="password"?"Show":"Hide";});
$("logout").addEventListener("click",()=>{sessionStorage.removeItem(SESSION_KEY);clearInterval(sessionInterval);$("app").classList.add("hidden");$("login-screen").classList.remove("hidden");$("password").value="";$("login-error").textContent="You have logged out.";});

function navigate(page) {
  document.querySelectorAll(".page").forEach(p=>p.classList.add("hidden"));
  $("page-"+page).classList.remove("hidden");
  document.querySelectorAll(".nav-item[data-page]").forEach(b=>b.classList.toggle("active",b.dataset.page===page));
  if(page==="games") drawIdleGame();
  if(page==="editor") renderDraftList();
  if(page==="code") renderSnippets();
}
document.querySelectorAll("[data-page]").forEach(b=>b.addEventListener("click",()=>navigate(b.dataset.page)));
document.querySelectorAll("[data-goto]").forEach(b=>b.addEventListener("click",()=>navigate(b.dataset.goto)));
$("today-label").textContent=new Date().toLocaleDateString(undefined,{weekday:"short",month:"short",day:"numeric"});

let snippets=DB.read("snippets",[]);
function renderSnippets() {
  const q=$("snippet-search").value.toLowerCase();
  const list=snippets.filter(s=>s.title.toLowerCase().includes(q)||s.code.toLowerCase().includes(q));
  $("snippet-grid").innerHTML="";
  list.forEach(s=>{
    const card=document.createElement("button");card.className="snippet-card";
    const top=document.createElement("div");top.className="snippet-card-top";
    const title=document.createElement("h3");title.textContent=s.title;
    const tag=document.createElement("span");tag.className="file-tag";tag.textContent="C++";
    top.append(title,tag);
    const preview=document.createElement("pre");preview.className="snippet-preview";preview.textContent=s.code;
    const foot=document.createElement("div");foot.className="snippet-card-footer";foot.innerHTML="<span>Click to copy</span><span>↗</span>";
    card.append(top,preview,foot);
    card.addEventListener("click",async()=>{try{await navigator.clipboard.writeText(s.code);toast("Copied "+s.title+" to clipboard");}catch{showSnippetText(s.code);}});
    card.addEventListener("contextmenu",e=>{e.preventDefault();if(confirm(`Delete "${s.title}"?`)){snippets=snippets.filter(x=>x.id!==s.id);DB.write("snippets",snippets);renderSnippets();}});
    $("snippet-grid").append(card);
  });
  $("snippet-empty").classList.toggle("hidden",list.length!==0);
  $("snippet-count").textContent=snippets.length;
}
$("snippet-search").addEventListener("input",renderSnippets);
function showSnippetText(code){$("snippet-editor").classList.remove("hidden");$("snippet-title").value="";$("snippet-code").value=code;$("snippet-editor").scrollIntoView({behavior:"smooth",block:"start"});toast("Clipboard unavailable — select and copy the code.");}
function openSnippetEditor(){ $("snippet-editor").classList.remove("hidden"); $("snippet-title").value=""; $("snippet-code").value=""; $("snippet-form-title").textContent="New snippet"; $("snippet-editor").scrollIntoView({behavior:"smooth",block:"start"}); }
$("new-snippet").addEventListener("click",openSnippetEditor);
$("close-snippet").addEventListener("click",()=>$("snippet-editor").classList.add("hidden"));
$("cancel-snippet").addEventListener("click",()=>$("snippet-editor").classList.add("hidden"));
$("save-snippet").addEventListener("click",()=>{const title=$("snippet-title").value.trim(),code=$("snippet-code").value;if(!title||!code.trim()){toast("Add a title and some code first.");return;}snippets.unshift({id:Date.now(),title,code});DB.write("snippets",snippets);$("snippet-editor").classList.add("hidden");renderSnippets();toast("Snippet saved.");});
function parseImportedJSON(text,filename){
  let obj;try{obj=JSON.parse(text);}catch{const first=text.indexOf("{"),last=text.lastIndexOf("}");if(first<0||last<=first)throw Error("Not valid JSON or a title + { code } format.");const title=text.slice(0,first).trim().split(/\r?\n/).filter(Boolean).pop()||filename;return {title,code:text.slice(first+1,last).trim()};}
  if(Array.isArray(obj))return obj.map((o,i)=>normaliseSnippet(o,filename+" "+(i+1)));
  return normaliseSnippet(obj,filename);
}
function normaliseSnippet(obj,fallback){if(typeof obj==="string")return {title:fallback,code:obj};const title=String(obj.title||obj.name||obj.label||fallback).replace(/\.json$/i,"");let code=obj.code??obj.cpp??obj.content??obj.source;if(code===undefined){const keys=Object.keys(obj).filter(k=>!["title","name","label"].includes(k));code=keys.length===1?obj[keys[0]]:JSON.stringify(obj,null,2);}if(typeof code!=="string")code=JSON.stringify(code,null,2);return {title,code};}
$("json-upload").addEventListener("change",async e=>{let added=0;for(const file of e.target.files){try{const parsed=parseImportedJSON(await file.text(),file.name.replace(/\.json$/i,""));const items=Array.isArray(parsed)?parsed:[parsed];items.forEach(s=>snippets.unshift({...s,id:Date.now()+Math.random()}));added+=items.length;}catch(err){toast(file.name+": "+err.message);}}DB.write("snippets",snippets);renderSnippets();e.target.value="";if(added)toast(`Imported ${added} snippet${added===1?"":"s"}.`);});

const defaultDraft=`#include <iostream>

int main() {
    std::cout << "Hello, world!\\n";
    return 0;
}`;
let drafts=DB.read("drafts",Array.from({length:10},(_,i)=>({name:`page-${i+1}.cpp`,code:i===0?defaultDraft:""})));
if(!Array.isArray(drafts)||drafts.length!==10)drafts=Array.from({length:10},(_,i)=>({name:`page-${i+1}.cpp`,code:""}));
let currentDraft=0,saveDebounce;
function renderDraftList(){const list=$("draft-list");list.innerHTML="";drafts.forEach((d,i)=>{const b=document.createElement("button");b.className="draft-button"+(i===currentDraft?" active":"");b.textContent=`page-${i+1}.cpp`;b.addEventListener("click",()=>switchDraft(i));list.append(b);});}
function switchDraft(i){drafts[currentDraft].code=$("cpp-code").value;currentDraft=i;$("cpp-code").value=drafts[i].code; $("current-file-name").textContent=drafts[i].name;renderDraftList();}
function saveDraft(){drafts[currentDraft].code=$("cpp-code").value;DB.write("drafts",drafts);$("save-status").textContent="Saved locally";renderDraftList();}
$("cpp-code").addEventListener("input",()=>{$("save-status").textContent="Saving…";clearTimeout(saveDebounce);saveDebounce=setTimeout(saveDraft,250);});
$("clear-draft").addEventListener("click",()=>{if(confirm("Clear this page?")){$("cpp-code").value="";saveDraft();$("code-output").value="";}});
function setupApp(){renderSnippets();renderDraftList();$("cpp-code").value=drafts[currentDraft].code;updateStats();}

function demoRun(source){
  if(/\b(?:while|for|do)\s*\(/.test(source))return "Demo runner doesn't execute loops. Connect a real compiler service to run full C++.";
  if(/\b(?:cin|scanf)\b/.test(source))return "This browser demo can't accept C++ stdin yet. Use a connected compiler service for interactive input.";
  const out=[];const re=/std::cout\s*<<\s*([\s\S]*?)(?=;|$)/g;let m;
  while((m=re.exec(source))!==null){let expr=m[1].trim();const parts=expr.split(/\s*<<\s*/);let line="";for(let p of parts){p=p.trim();if(/^".*"$/.test(p)){line+=p.slice(1,-1).replace(/\\n/g,"\n").replace(/\\t/g,"\t").replace(/\\"/g,'"').replace(/\\\\/g,"\\");}else if(/^'.*'$/.test(p)){line+=p.slice(1,-1);}else if(/^-?\d+(?:\.\d+)?$/.test(p)){line+=p;}else if(p==="std::endl"){line+="\n";}else if(p==="true"||p==="false"){line+=p==="true"?"1":"0";}else if(p===""){continue;}else{return "Demo runner supports simple std::cout << \"text\"; statements only.\nUnsupported expression: "+p;}}out.push(line);}
  if(out.length)return out.join("");
  if(/int\s+main\s*\(/.test(source))return "(Program finished. No supported std::cout output found.)";
  return "No output. Add a main() function and std::cout statement.";
}
$("run-code").addEventListener("click",async()=>{const source=$("cpp-code").value;const runButton=$("run-code");runButton.disabled=true;runButton.textContent="Compiling…";$("compiler-status").textContent="Contacting online compiler…";$("code-output").value="";saveDraft();try{if(!window.SamuelsCppCompiler)throw new Error("Compiler script did not load. Refresh the page and try again.");const result=await window.SamuelsCppCompiler.compileCpp({code:source,stdin:$("code-input").value});const pieces=[];if(result.error)pieces.push(result.error);if(result.output)pieces.push(result.output);$("code-output").value=pieces.join("\n")||(result.ok?"(Program finished with no output.)":"Compilation failed with no output.");$("compiler-status").textContent=result.status+(result.compiler?" · "+result.compiler:"");}catch(error){$("code-output").value=error.message||"Could not run the compiler.";$("compiler-status").textContent="Compiler unavailable";}finally{runButton.disabled=false;runButton.textContent="▶ Compile & run";saveDraft();}});
$("export-draft").addEventListener("click",()=>{const code=$("cpp-code").value;const blob=new Blob([code],{type:"text/x-c++src;charset=utf-8"});const url=URL.createObjectURL(blob);const link=document.createElement("a");link.href=url;link.download=drafts[currentDraft].name||("page-"+(currentDraft+1)+".cpp");document.body.append(link);link.click();link.remove();URL.revokeObjectURL(url);toast("C++ draft exported.");});
function updateStats(){$("snippet-count").textContent=snippets.length;$("best-score").textContent=DB.read("best_space",0);$("coins").textContent=DB.read("coins",0);}

const canvas=$("game-canvas"),ctx=canvas.getContext("2d");
let gameType="space",gameRunning=false,animId=0,lastTime=0,score=0,player={x:150,y:220,vy:0},obstacles=[],particles=[],hold=false,skin=DB.read("skin","#9c8cff"),clickCount=0,clickStart=0,memCards=[],memOpen=[],memMatched=[],memMoves=0,memorySquareCount=DB.read("memory_size",16);
document.querySelectorAll(".skin-choice").forEach(b=>{b.style.setProperty("--skin",b.dataset.skin);b.classList.toggle("selected",b.dataset.skin===skin);b.addEventListener("click",()=>{skin=b.dataset.skin;DB.write("skin",skin);document.querySelectorAll(".skin-choice").forEach(x=>x.classList.toggle("selected",x===b));});});
function setGame(type){
  gameType=type;gameRunning=false;cancelAnimationFrame(animId);
  document.querySelectorAll(".game-tab").forEach(b=>b.classList.toggle("active",b.dataset.game===type));
  const names={
    space:["Sky Drift","Hold to rise. Release to fall. Dodge the gates.","A one-button flight challenge. Keep the little ship between the gates for as long as you can."],
    rider:["Neon Rider","Tap the game or press Space to jump over barriers.","Jump over the glowing barriers and stay on the winding neon track."],
    clicker:["Quick Tap","Tap the target as many times as possible in 10 seconds.","A quick reaction game. Tap the target before time runs out."],
    memory:["Memory Grid","Match every pair in as few moves as possible.","Choose 8, 16 or 32 squares. Your score is based on the minimum possible moves."]
  };
  $("overlay-title").textContent=names[type][0];$("overlay-desc").textContent=names[type][1];$("game-name").textContent=names[type][0];$("game-description").textContent=names[type][2];
  $("start-game").textContent=type==="clicker"?"Start 10-second round":type==="memory"?"Start memory game":"Start game ↗";
  $("game-score").textContent=type==="memory"?"Score 0":"Score 0";
  $("game-best").textContent="Best "+DB.read("best_"+type,0);
  $("game-overlay").classList.remove("hidden");
  $("memory-mode").classList.toggle("hidden",type!=="memory");
  document.querySelector("#control-primary span:first-child").textContent=type==="space"?"SPACE / HOLD":type==="rider"?"TAP / SPACE":type==="clicker"?"TAP TARGET":"TAP PAIRS";
  document.querySelector("#control-primary span:last-child").textContent=type==="space"?"Rise up":type==="rider"?"Jump":type==="clicker"?"Score a tap":"Flip a square";
  document.querySelector("#control-secondary span:first-child").textContent=type==="space"?"RELEASE":type==="rider"?"LAND":type==="clicker"?"10 SECONDS":"MATCH";
  document.querySelector("#control-secondary span:last-child").textContent=type==="space"?"Fall down":type==="rider"?"Jump again":type==="clicker"?"Beat your best":"Find pairs";
  document.querySelectorAll("[data-memory-size]").forEach(b=>b.classList.toggle("active",Number(b.dataset.memorySize)===memorySquareCount));
  updateMemoryModeHint();
  drawIdleGame();
}
document.querySelectorAll("[data-game]").forEach(b=>b.addEventListener("click",()=>setGame(b.dataset.game)));
function drawBackground(){ctx.fillStyle="#0e1016";ctx.fillRect(0,0,800,450);ctx.strokeStyle="#202330";ctx.lineWidth=1;for(let x=0;x<800;x+=40){ctx.beginPath();ctx.moveTo(x,0);ctx.lineTo(x,450);ctx.stroke();}for(let y=0;y<450;y+=40){ctx.beginPath();ctx.moveTo(0,y);ctx.lineTo(800,y);ctx.stroke();}for(let i=0;i<45;i++){let x=(i*173+27)%800,y=(i*89+13)%450;ctx.fillStyle="#646a85";ctx.globalAlpha=.35+(i%4)*.12;ctx.fillRect(x,y,2,2);}ctx.globalAlpha=1;}
function drawIdleGame(){if(!canvas||gameRunning)return;drawBackground();ctx.fillStyle=skin;ctx.beginPath();ctx.arc(150,220,10,0,Math.PI*2);ctx.fill();ctx.fillStyle="#a2a6b9";ctx.font="14px monospace";ctx.fillText(gameType==="rider"?"NEON RIDER":gameType==="memory"?"MEMORY GRID":gameType==="clicker"?"QUICK TAP":"SKY DRIFT",28,42);if(gameType==="rider"){ctx.strokeStyle=skin;ctx.lineWidth=4;ctx.beginPath();ctx.moveTo(0,340);for(let x=0;x<800;x+=10)ctx.lineTo(x,340+Math.sin(x*.02)*35+Math.sin(x*.05)*10);ctx.stroke();}if(gameType==="memory"){const g=memoryLayout(memorySquareCount);for(let i=0;i<memorySquareCount;i++){const x=g.x+(i%g.cols)*(g.tileW+g.gap),y=g.y+Math.floor(i/g.cols)*(g.tileH+g.gap);ctx.fillStyle="#29273b";ctx.fillRect(x,y,g.tileW,g.tileH);}}if(gameType==="clicker"){ctx.fillStyle=skin;ctx.beginPath();ctx.arc(400,225,42,0,Math.PI*2);ctx.fill();}}
function startGame(){cancelAnimationFrame(animId);score=0;lastTime=0;gameRunning=true;hold=false;player={x:150,y:220,vy:0};obstacles=[];particles=[];$("game-overlay").classList.add("hidden");if(gameType==="memory"){startMemory();return;}if(gameType==="clicker"){startClicker();return;}if(gameType==="rider"){player={x:170,y:300,vy:0};}animId=requestAnimationFrame(loopGame);}
$("start-game").addEventListener("click",startGame);
function loopGame(time){if(!gameRunning)return;if(!lastTime)lastTime=time;const dt=Math.min((time-lastTime)/16.67,2);lastTime=time;score+=dt*.12;if(gameType==="space")stepSpace(dt);else stepRider(dt);drawGame();$("game-score").textContent="Score "+Math.floor(score);animId=requestAnimationFrame(loopGame);}
function stepSpace(dt){player.vy+=(hold?-.48:.42)*dt;player.vy=Math.max(-5.4,Math.min(5.4,player.vy));player.y+=player.vy*dt;if(player.y<10||player.y>440){endGame();return;}if(Math.random()<.025*dt){let gap=125+Math.random()*45,top=35+Math.random()*(270-gap);obstacles.push({x:830,top,gap,w:40,passed:false});}obstacles.forEach(o=>{o.x-=4.1*dt;if(!o.passed&&o.x+o.w<player.x){o.passed=true;score+=2;DB.write("coins",DB.read("coins",0)+1);updateStats();}if(player.x+8>o.x&&player.x-8<o.x+o.w&&(player.y-8<o.top||player.y+8>o.top+o.gap))endGame();});obstacles=obstacles.filter(o=>o.x>-50);}
let riderX=0;
function stepRider(dt){riderX+=4*dt;player.vy+=.42*dt;player.y+=player.vy*dt;const ground=320+Math.sin((riderX+player.x)*.018)*34+Math.sin((riderX+player.x)*.047)*12;if(player.y>ground-13){player.y=ground-13;player.vy=0;}if(player.y>430){endGame();return;}if(Math.random()<.02*dt){const obstacleGround=320+Math.sin((riderX+830)*.018)*34+Math.sin((riderX+830)*.047)*12;obstacles.push({x:830,y:obstacleGround-35,w:23,h:35});}obstacles.forEach(o=>o.x-=5*dt);obstacles=obstacles.filter(o=>o.x>-30);for(const o of obstacles){if(Math.abs(o.x-player.x)<22&&player.y>o.y-10){endGame();return;}}}
function riderJump(){if(!gameRunning||gameType!=="rider")return;const ground=320+Math.sin((riderX+player.x)*.018)*34+Math.sin((riderX+player.x)*.047)*12;if(player.y>=ground-27&&player.vy>=-1){player.vy=-8.5;}}
function drawGame(){drawBackground();ctx.strokeStyle=skin;ctx.lineWidth=4;ctx.shadowBlur=14;ctx.shadowColor=skin;if(gameType==="space"){obstacles.forEach(o=>{ctx.fillStyle="#353049";ctx.fillRect(o.x,0,o.w,o.top);ctx.fillRect(o.x,o.top+o.gap,o.w,450-o.top-o.gap);ctx.fillStyle=skin;ctx.fillRect(o.x,o.top-3,o.w,3);ctx.fillRect(o.x,o.top+o.gap,o.w,3);});ctx.fillStyle=skin;ctx.beginPath();ctx.arc(player.x,player.y,9,0,Math.PI*2);ctx.fill();}else{ctx.beginPath();ctx.moveTo(0,320);for(let x=0;x<=800;x+=5)ctx.lineTo(x,320+Math.sin((riderX+x)*.018)*34+Math.sin((riderX+x)*.047)*12);ctx.stroke();obstacles.forEach(o=>{ctx.fillStyle="#ffad66";ctx.fillRect(o.x,o.y,o.w,o.h);});ctx.fillStyle=skin;ctx.beginPath();ctx.arc(player.x,player.y,11,0,Math.PI*2);ctx.fill();ctx.fillStyle="#d8dbe6";ctx.beginPath();ctx.arc(player.x-8,player.y+12,4,0,Math.PI*2);ctx.arc(player.x+8,player.y+12,4,0,Math.PI*2);ctx.fill();}ctx.shadowBlur=0;}
function endGame(){if(!gameRunning)return;gameRunning=false;cancelAnimationFrame(animId);const final=Math.floor(score);const key="best_"+gameType;const old=DB.read(key,0);if(final>old)DB.write(key,final);DB.write("best_space",Math.max(DB.read("best_space",0),gameType==="space"?final:0));DB.write("coins",DB.read("coins",0)+Math.floor(final/3));updateStats();$("game-best").textContent="Best "+Math.max(old,final);$("overlay-title").textContent=gameType==="space"?"Flight over":gameType==="rider"?"Ride over":gameType==="memory"?"Grid complete":"Round over";$("overlay-desc").textContent=gameType==="memory"?`You scored ${final} points in ${memMoves} moves. ${final>old?"New personal best!":"Try to get closer to the minimum moves."}`:`You scored ${final}. ${final>old?"New personal best!":"Try another run and beat your record."}`;$("start-game").textContent="Play again";$("game-overlay").classList.remove("hidden");}
function setHold(value){hold=value;}
canvas.addEventListener("pointerdown",e=>{e.preventDefault();if(!gameRunning)return;if(gameType==="clicker"){clickCount++;score=clickCount;$("game-score").textContent="Taps "+clickCount;return;}if(gameType==="memory"){memoryClick(e);return;}if(gameType==="rider"){riderJump();return;}setHold(true);},{passive:false});
canvas.addEventListener("contextmenu",e=>e.preventDefault());
window.addEventListener("pointerup",()=>setHold(false));window.addEventListener("pointercancel",()=>setHold(false));window.addEventListener("keydown",e=>{if(e.code==="Space"&&gameRunning){e.preventDefault();if(gameType==="rider")riderJump();else if(gameType==="space")setHold(true);}});window.addEventListener("keyup",e=>{if(e.code==="Space")setHold(false);});

function startClicker(){clickCount=0;clickStart=Date.now();const tick=()=>{if(!gameRunning||gameType!=="clicker")return;const left=Math.max(0,10-Math.floor((Date.now()-clickStart)/1000));drawBackground();ctx.fillStyle=skin;ctx.beginPath();ctx.arc(400,225,55,0,Math.PI*2);ctx.fill();ctx.fillStyle="#161520";ctx.font="bold 28px sans-serif";ctx.textAlign="center";ctx.fillText("TAP",400,234);ctx.textAlign="left";$("game-score").textContent=`Taps ${clickCount} · ${left}s`;if(left<=0){score=clickCount;endGame();return;}animId=requestAnimationFrame(tick);};tick();}
const memorySymbols=["◆","●","▲","✦","⬟","✚","☾","✿","♥","⬢","★","✧","☀","♣","♫","❖"];
function memoryLayout(size){const cols=size===32?8:4,rows=Math.ceil(size/cols),gap=size===32?8:12,tileW=size===32?72:112,tileH=rows===2?120:72,totalW=cols*tileW+(cols-1)*gap,totalH=rows*tileH+(rows-1)*gap;return {cols,rows,gap,tileW,tileH,x:(800-totalW)/2,y:(450-totalH)/2+10};}
function startMemory(){const pairs=memorySquareCount/2;const symbols=[...memorySymbols];while(symbols.length<pairs)symbols.push("✦");memCards=[...symbols.slice(0,pairs),...symbols.slice(0,pairs)].sort(()=>Math.random()-.5);memOpen=[];memMatched=[];memMoves=0;score=0;drawMemory();$("game-score").textContent="Moves 0 · Score 0";$("game-best").textContent="Best "+DB.read("best_memory",0);}
function drawMemory(){drawBackground();const g=memoryLayout(memorySquareCount);for(let i=0;i<memorySquareCount;i++){const col=i%g.cols,row=Math.floor(i/g.cols),x=g.x+col*(g.tileW+g.gap),y=g.y+row*(g.tileH+g.gap),open=memOpen.includes(i)||memMatched.includes(i);ctx.fillStyle=open?"#39334f":"#242735";ctx.fillRect(x,y,g.tileW,g.tileH);ctx.strokeStyle=open?skin:"#3b4050";ctx.lineWidth=2;ctx.strokeRect(x,y,g.tileW,g.tileH);if(open){ctx.fillStyle=skin;ctx.font=(memorySquareCount===32?"28px":"36px")+" sans-serif";ctx.textAlign="center";ctx.fillText(memCards[i],x+g.tileW/2,y+g.tileH/2+12);ctx.textAlign="left";}}}
function memoryClick(e){const rect=canvas.getBoundingClientRect(),x=(e.clientX-rect.left)*800/rect.width,y=(e.clientY-rect.top)*450/rect.height,g=memoryLayout(memorySquareCount),col=Math.floor((x-g.x)/(g.tileW+g.gap)),row=Math.floor((y-g.y)/(g.tileH+g.gap));if(col<0||col>=g.cols||row<0||row>=g.rows||x>g.x+g.cols*g.tileW+(g.cols-1)*g.gap||y>g.y+g.rows*g.tileH+(g.rows-1)*g.gap)return;const i=row*g.cols+col;if(i>=memorySquareCount||memOpen.length>=2||memMatched.includes(i)||memOpen.includes(i))return;memOpen.push(i);if(memOpen.length===2){memMoves++;if(memCards[memOpen[0]]===memCards[memOpen[1]]){memMatched.push(...memOpen);memOpen=[];const matchedPairs=memMatched.length/2;score=Math.floor(1000*(memorySquareCount/2)/Math.max(memorySquareCount/2,memMoves));$("game-best").textContent="Best "+DB.read("best_memory",0);if(memMatched.length===memorySquareCount){score=Math.floor(1000*(memorySquareCount/2)/memMoves);$("game-score").textContent="Moves "+memMoves+" · Score "+score;setTimeout(()=>{if(gameRunning&&gameType==="memory")endGame();},350);}}else{setTimeout(()=>{if(gameRunning&&gameType==="memory"){memOpen=[];drawMemory();}},650);}}$("game-score").textContent="Moves "+memMoves+" · Score "+score;drawMemory();}
function updateMemoryModeHint(){document.querySelector(".memory-score-hint").textContent="Perfect score: 1000 points in "+(memorySquareCount/2)+" moves.";$("overlay-desc").textContent="Match "+(memorySquareCount/2)+" pairs. Perfect score: 1000 points in "+(memorySquareCount/2)+" moves.";}
document.querySelectorAll("[data-memory-size]").forEach(b=>b.addEventListener("click",()=>{memorySquareCount=Number(b.dataset.memorySize);DB.write("memory_size",memorySquareCount);document.querySelectorAll("[data-memory-size]").forEach(x=>x.classList.toggle("active",x===b));updateMemoryModeHint();if(gameType==="memory"){gameRunning=false;cancelAnimationFrame(animId);$("game-overlay").classList.remove("hidden");$("overlay-title").textContent="Memory Grid · "+memorySquareCount+" squares";drawIdleGame();}}));
function updateStats(){$("snippet-count").textContent=snippets.length;$("best-score").textContent=DB.read("best_space",0);$("coins").textContent=DB.read("coins",0);}

// Resume an existing session only after all app data and click handlers are initialized.
if(Number(sessionStorage.getItem(SESSION_KEY)||0)>Date.now())showApp();
drawIdleGame();