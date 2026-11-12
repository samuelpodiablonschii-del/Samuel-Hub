(() => {
"use strict";
let modPromise=null,mod=null,state=null,frame=0,runStart=0;
const imageCache=new Map();
const $=id=>document.getElementById(id);
const canvas=()=>$("trg-ui-canvas");
const output=()=>$("code-output");
const status=s=>{if($("compiler-status"))$("compiler-status").textContent=s;};
function ptr(s){return mod.stringToNewUTF8(String(s??""));}
function str(name){const p=mod[name]();return p?mod.UTF8ToString(p):"";}
async function init(){
 if(mod)return mod;
 if(modPromise)return modPromise;
 if(typeof window.createTRGModule!=="function")throw Error("TRG WebAssembly has not been built yet. Wait for the build workflow, then refresh.");
 modPromise=window.createTRGModule({print:s=>{if(output())output().value+=s+"\n";},printErr:s=>{if(output())output().value+=s+"\n";}});
 mod=await modPromise;
 try{mod.FS.mkdir("/trg-project");}catch(_){}
 mod.FS.chdir("/trg-project");
 status("TRG WebAssembly ready");
 return mod;
}
function groupVisible(name,groups,seen=new Set()){
 if(!name||name==="default")return true;
 if(seen.has(name))return false;
 seen.add(name);
 const g=groups.find(x=>x.name===name);
 return !!g&&g.visible!==false&&groupVisible(g.parent,groups,seen);
}
function rounded(ctx,x,y,w,h,r){
 r=Math.max(0,Math.min(r||0,w/2,h/2));ctx.beginPath();ctx.moveTo(x+r,y);ctx.arcTo(x+w,y,x+w,y+h,r);ctx.arcTo(x+w,y+h,x,y+h,r);ctx.arcTo(x,y+h,x,y,r);ctx.arcTo(x,y,x+w,y,r);ctx.closePath();
}
function poly(ctx,x,y,w,h,n,star=false){
 ctx.beginPath();const count=star?n*2:n;
 for(let i=0;i<count;i++){const a=-Math.PI/2+i*Math.PI*2/count,r=star&&i%2?.22:.5,px=x+w/2+Math.cos(a)*w*r,py=y+h/2+Math.sin(a)*h*r;if(!i)ctx.moveTo(px,py);else ctx.lineTo(px,py);}ctx.closePath();
}
function position(item,now){
 let x=item.x||0,y=item.y||0;
 const a=(state.animations||[]).find(v=>v.name===item.name);
 if(a){const elapsed=Math.max(0,(now-runStart)/1000-(a.startTime||0)),p=a.duration>0?Math.min(1,elapsed/a.duration):1;x=(a.startX||x)+(a.dx||0)*p;y=(a.startY||y)+(a.dy||0)*p;}
 return{x,y};
}
async function sprite(path){
 if(imageCache.has(path))return imageCache.get(path);
 const p=(async()=>{try{const bytes=mod.FS.readFile("/trg-project/"+path),ext=path.split(".").pop().toLowerCase(),mime=({png:"image/png",jpg:"image/jpeg",jpeg:"image/jpeg",gif:"image/gif",webp:"image/webp"})[ext]||"application/octet-stream",url=URL.createObjectURL(new Blob([bytes],{type:mime})),img=new Image();img.src=url;await img.decode();return img;}catch(_){return null;}})();
 imageCache.set(path,p);return p;
}
async function draw(){
 const c=canvas();if(!c||!state||!mod)return;
 if(mod._trg_tick){mod._trg_tick();try{state=JSON.parse(str("_trg_get_ui_state")||"{}");}catch(_){}}\n const ctx=c.getContext("2d"),W=c.width,H=c.height,now=performance.now(),groups=state.groups||[],visible=v=>v.visible!==false&&groupVisible(v.group,groups);
 ctx.clearRect(0,0,W,H);ctx.fillStyle="#11131b";ctx.fillRect(0,0,W,H);
 for(const v of [...(state.blocks||[])].sort((a,b)=>(a.z||0)-(b.z||0))){
  if(!visible(v))continue;const p=position(v,now),x=p.x*W/100,y=p.y*H/100,w=v.width*W/100,h=v.height*H/100;
  ctx.save();ctx.globalAlpha=v.opacity??1;ctx.fillStyle=v.color||"#7043be";
  if(v.shape==="circle"){ctx.beginPath();ctx.ellipse(x+w/2,y+h/2,w/2,h/2,0,0,Math.PI*2);ctx.fill();}
  else if(v.shape==="triangle"||v.shape==="pentagon"||v.shape==="hexagon"||v.shape==="star"){poly(ctx,x,y,w,h,v.shape==="triangle"?3:v.shape==="pentagon"?5:6,v.shape==="star");ctx.fill();}
  else{rounded(ctx,x,y,w,h,v.cornerRadius||0);ctx.fill();}ctx.restore();
 }
 for(const v of state.sprites||[]){if(!visible(v))continue;const img=await sprite(v.path);if(!img)continue;const p=position(v,now);ctx.save();ctx.globalAlpha=v.opacity??1;ctx.drawImage(img,p.x*W/100,p.y*H/100,v.width*W/100,v.height*H/100);ctx.restore();}
 for(const v of state.texts||[]){if(!visible(v))continue;const p=position(v,now);ctx.save();ctx.globalAlpha=v.opacity??1;ctx.fillStyle=v.color||"#fff";ctx.font=(v.bold?"700 ":"400 ")+Math.max(10,v.size*H/100)+"px "+(v.font==="mono"?"monospace":v.font==="serif"?"serif":"system-ui");ctx.textAlign=v.align||"left";ctx.textBaseline="middle";ctx.fillText(v.value||"",p.x*W/100,p.y*H/100);ctx.restore();}
 for(const v of state.buttons||[]){if(!visible(v))continue;const x=v.x*W/100,y=v.y*H/100,w=v.width*W/100,h=v.height*H/100;ctx.save();ctx.globalAlpha=v.opacity??1;ctx.fillStyle=v.color||"#7043be";rounded(ctx,x,y,w,h,v.cornerRadius??7);ctx.fill();ctx.fillStyle=v.textColor||"#fff";ctx.font="600 18px system-ui";ctx.textAlign="center";ctx.textBaseline="middle";ctx.fillText(v.label||"",x+w/2,y+h/2,Math.max(12,w-12));ctx.restore();}
 frame=requestAnimationFrame(draw);
}
function render(){if(frame)cancelAnimationFrame(frame);frame=requestAnimationFrame(draw);}
function refreshState(){state=JSON.parse(str("_trg_get_ui_state")||"{}");const web=str("_trg_get_web_html");const iframe=$("trg-web-frame");if(iframe){iframe.srcdoc=web||"";iframe.classList.toggle("hidden",!web);}if(canvas())canvas().classList.toggle("hidden",!!web);render();}
async function run(source,input=""){
 await init();const a=ptr(source),b=ptr(input);
 try{const p=mod._trg_run_source(a,b),text=p?mod.UTF8ToString(p):"";refreshState();return{output:text,state,status:"TRG finished"};}
 finally{mod._free(a);mod._free(b);}
}
async function addFiles(files){
 await init();let count=0;
 for(const file of files){const path=(file.webkitRelativePath||file.name).replace(/\\/g,"/");if(!path||path.startsWith("/")||path.split("/").some(x=>!x||x==="."||x===".."))continue;const parts=path.split("/");let dir="/trg-project";for(const part of parts.slice(0,-1)){dir+="/"+part;try{mod.FS.mkdir(dir);}catch(_){}}mod.FS.writeFile("/trg-project/"+path,new Uint8Array(await file.arrayBuffer()));count++;}
 return count;
}
function clickButton(e){
 if(!mod||!state)return;const c=canvas();if(!c)return;const r=c.getBoundingClientRect(),x=(e.clientX-r.left)/r.width*100,y=(e.clientY-r.top)/r.height*100,groups=state.groups||[];
 const b=(state.buttons||[]).slice().reverse().find(v=>v.visible!==false&&groupVisible(v.group,groups)&&x>=v.x&&x<=v.x+v.width&&y>=v.y&&y<=v.y+v.height);if(!b)return;
 const p=ptr(b.name);try{mod._trg_click_button(p);refreshState();if(output())output().value=str("_trg_get_output");}finally{mod._free(p);}
}
const keys={Space:"key_space",Enter:"key_enter",Escape:"key_escape",Tab:"key_tab",ArrowLeft:"key_left",ArrowRight:"key_right",ArrowUp:"key_up",ArrowDown:"key_down",Backspace:"key_backspace",Delete:"key_delete"};
function keyName(e){if(keys[e.code])return keys[e.code];if(/^Key[A-Z]$/.test(e.code))return"key_"+e.code.slice(3).toLowerCase();if(/^Digit[0-9]$/.test(e.code))return"key_"+e.code.slice(5);if(/^F([1-9]|1[0-9]|2[0-4])$/.test(e.key))return"key_"+e.key.toLowerCase();return"";}
window.addEventListener("keydown",e=>{const k=keyName(e);if(!k||!mod)return;const p=ptr(k);try{mod._trg_set_key(p,1);}finally{mod._free(p);}});
window.addEventListener("keyup",e=>{const k=keyName(e);if(!k||!mod)return;const p=ptr(k);try{mod._trg_set_key(p,0);}finally{mod._free(p);}});
document.addEventListener("DOMContentLoaded",()=>{
 canvas()?.addEventListener("pointerdown",e=>{e.preventDefault();clickButton(e);});
 $("trg-files")?.addEventListener("change",async e=>{try{const n=await addFiles(e.target.files);status(n+" project file(s) imported");}catch(err){status("Import failed: "+err.message);}e.target.value="";});
});
window.TRGWeb=Object.freeze({init,run,addFiles,render});
})();