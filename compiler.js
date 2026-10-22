(() => {
  "use strict";

  const API = "https://wandbox.org/api";
  const REQUEST_TIMEOUT_MS = 25000;
  let compilerPromise;

  async function fetchJson(url, options = {}) {
    const controller = new AbortController();
    const timer = setTimeout(() => controller.abort(), REQUEST_TIMEOUT_MS);
    try {
      const response = await fetch(url, {
        ...options,
        signal: controller.signal,
        headers: { "Content-Type": "application/json", ...(options.headers || {}) }
      });
      if (!response.ok) {
        const detail = await response.text().catch(() => "");
        throw new Error("Compiler service returned HTTP " + response.status + (detail ? ": " + detail.slice(0, 300) : ""));
      }
      return await response.json();
    } catch (error) {
      if (error.name === "AbortError") throw new Error("Compilation timed out while contacting the online compiler.");
      if (error instanceof TypeError) throw new Error("Could not reach the online compiler. Check your internet connection or try again later.");
      throw error;
    } finally {
      clearTimeout(timer);
    }
  }

  async function findCompiler() {
    if (!compilerPromise) {
      const cached = (() => { try { return localStorage.getItem("samuelhub_compiler_name"); } catch { return null; } })();
      if (cached) return Promise.resolve(cached);
      compilerPromise = fetchJson(API + "/list.json").then(list => {
        const compilers = Array.isArray(list) ? list : [];
        const candidates = compilers.filter(item =>
          item && typeof item.name === "string" &&
          /^(?:gcc|g\+\+|gnu c\+\+)/i.test(item.name) &&
          /c\+\+|gcc|g\+\+/i.test(String(item.language || item.name))
        );
        if (!candidates.length) throw new Error("No C++ compiler is currently available from the online service.");
        candidates.sort((a, b) => {
          const version = id => {
            const match = String(id || "").match(/(\d+)(?:\.(\d+))?/);
            return match ? Number(match[1]) * 100 + Number(match[2] || 0) : (/(?:head|trunk|latest)/i.test(String(id || "")) ? 9999 : 0);
          };
          return version(b.name) - version(a.name);
        });
        const selected = candidates[0].name;
        try { localStorage.setItem("samuelhub_compiler_name", selected); } catch {}
        return selected;
      }).catch(error => {
        compilerPromise = null;
        try { localStorage.removeItem("samuelhub_compiler_name"); } catch {}
        throw error;
      });
    }
    return compilerPromise;
  }

  async function compileCpp({ code, stdin = "" } = {}) {
    if (typeof code !== "string" || !code.trim()) {
      return { ok: false, output: "", error: "Write some C++ code before running it.", status: "No source code" };
    }

    try {
      const compiler = await findCompiler();
      const result = await fetchJson(API + "/compile.json", {
        method: "POST",
        body: JSON.stringify({
          compiler,
          code,
          stdin: String(stdin ?? ""),
          "compiler-option-raw": "-std=gnu++23",
          "runtime-option-raw": "",
          options: "warning,gnu++23"
        })
      });

      const compilerError = String(result.compiler_error || "").trim();
      const programOutput = String(result.program_output || "");
      const programError = String(result.program_error || "").trim();
      const exitCode = result.status === undefined ? null : result.status;
      const output = [programOutput, programError ? "[stderr]\n" + programError : ""].filter(Boolean).join("\n");
      const failed = Boolean(compilerError) || (typeof exitCode === "number" && exitCode !== 0) || Boolean(result.signal);
      return {
        ok: !failed,
        output,
        error: compilerError,
        status: failed ? "Compilation or execution failed" : "Finished successfully",
        compiler,
        exitCode,
        signal: result.signal || null
      };
    } catch (error) {
      return {
        ok: false,
        output: "",
        error: error instanceof Error ? error.message : "An unexpected compiler error occurred.",
        status: "Compiler unavailable"
      };
    }
  }

  window.SamuelsCppCompiler = Object.freeze({ compileCpp });
})();
