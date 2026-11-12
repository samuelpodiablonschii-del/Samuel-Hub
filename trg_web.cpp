#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <cmath>
#include <random>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <limits>
#include <map>
#include <system_error>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cwchar>
#include <variant>
#include <thread>
#include <atomic>
#include <mutex>
#include <stdexcept>
#include <memory>
#include <array>
#include <deque>

#ifdef _WIN32
#include <windows.h>
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")
#endif

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <commdlg.h>
#include <shellapi.h>
#include <gdiplus.h>
#include "resource.h"
#elif defined(__ANDROID__)
#include <jni.h>
#if defined(TRG_BOUNCYBALLS_EXTENSION)
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "GameCore.h"
#endif
#define VK_SPACE 0x20
#define VK_RETURN 0x0D
#define VK_ESCAPE 0x1B
#define VK_TAB 0x09
#define VK_SHIFT 0x10
#define VK_CONTROL 0x11
#define VK_MENU 0x12
#define VK_LEFT 0x25
#define VK_UP 0x26
#define VK_RIGHT 0x27
#define VK_DOWN 0x28
#define VK_F1 0x70
#define VK_NUMPAD0 0x60
#define VK_BACK 0x08
#define VK_DELETE 0x2E
#define VK_INSERT 0x2D
#define VK_HOME 0x24
#define VK_END 0x23
#define VK_PRIOR 0x21
#define VK_NEXT 0x22
#define VK_CAPITAL 0x14
#define VK_NUMLOCK 0x90
#define VK_SCROLL 0x91
#define VK_SNAPSHOT 0x2C
#define VK_PAUSE 0x13
#define VK_LWIN 0x5B
#define VK_RWIN 0x5C
#define VK_APPS 0x5D
#define VK_ADD 0x6B
#define VK_SUBTRACT 0x6D
#define VK_MULTIPLY 0x6A
#define VK_DIVIDE 0x6F
#define VK_DECIMAL 0x6E
#define VK_OEM_1 0xBA
#define VK_OEM_PLUS 0xBB
#define VK_OEM_COMMA 0xBC
#define VK_OEM_MINUS 0xBD
#define VK_OEM_PERIOD 0xBE
#define VK_OEM_2 0xBF
#define VK_OEM_3 0xC0
#define VK_OEM_4 0xDB
#define VK_OEM_5 0xDC
#define VK_OEM_6 0xDD
#define VK_OEM_7 0xDE
#endif

#if !defined(_WIN32) && !defined(__ANDROID__)
#ifndef VK_SPACE
#define VK_SPACE 0x20
#define VK_RETURN 0x0D
#define VK_ESCAPE 0x1B
#define VK_TAB 0x09
#define VK_SHIFT 0x10
#define VK_CONTROL 0x11
#define VK_MENU 0x12
#define VK_LEFT 0x25
#define VK_UP 0x26
#define VK_RIGHT 0x27
#define VK_DOWN 0x28
#define VK_F1 0x70
#define VK_NUMPAD0 0x60
#define VK_BACK 0x08
#define VK_DELETE 0x2E
#define VK_INSERT 0x2D
#define VK_HOME 0x24
#define VK_END 0x23
#define VK_PRIOR 0x21
#define VK_NEXT 0x22
#define VK_CAPITAL 0x14
#define VK_NUMLOCK 0x90
#define VK_SCROLL 0x91
#define VK_SNAPSHOT 0x2C
#define VK_PAUSE 0x13
#define VK_LWIN 0x5B
#define VK_RWIN 0x5C
#define VK_APPS 0x5D
#define VK_ADD 0x6B
#define VK_SUBTRACT 0x6D
#define VK_MULTIPLY 0x6A
#define VK_DIVIDE 0x6F
#define VK_DECIMAL 0x6E
#define VK_OEM_1 0xBA
#define VK_OEM_PLUS 0xBB
#define VK_OEM_COMMA 0xBC
#define VK_OEM_MINUS 0xBD
#define VK_OEM_PERIOD 0xBE
#define VK_OEM_2 0xBF
#define VK_OEM_3 0xC0
#define VK_OEM_4 0xDB
#define VK_OEM_5 0xDC
#define VK_OEM_6 0xDD
#define VK_OEM_7 0xDE
#endif
#endif

#if defined(TRG_WEB)
static std::string trgWebOutput;
static std::vector<std::string> trgWebInputLines;
static std::size_t trgWebInputIndex = 0;
static std::unordered_set<std::string> trgWebPressedKeys;
#endif

struct VariableValue {
    std::variant<double, bool, std::string, std::vector<std::string>> value = std::string();
    VariableValue() = default;
    VariableValue(const std::string& text) : value(text) {}
    VariableValue(std::string&& text) : value(std::move(text)) {}
    VariableValue(const char* text) : value(std::string(text)) {}
    VariableValue(double number) : value(number) {}
    VariableValue(bool flag) : value(flag) {}
    VariableValue(const std::vector<std::string>& items) : value(items) {}
    VariableValue(std::vector<std::string>&& items) : value(std::move(items)) {}
    VariableValue& operator=(const std::string& text) { value = text; return *this; }
    VariableValue& operator=(std::string&& text) { value = std::move(text); return *this; }
    VariableValue& operator=(const char* text) { value = std::string(text); return *this; }
    VariableValue& operator=(double number) { value = number; return *this; }
    VariableValue& operator=(bool flag) { value = flag; return *this; }
    VariableValue& operator=(const std::vector<std::string>& items) { value = items; return *this; }
    VariableValue& operator=(std::vector<std::string>&& items) { value = std::move(items); return *this; }
    operator std::string() const {
        if (const auto* text = std::get_if<std::string>(&value)) return *text;
        if (const auto* number = std::get_if<double>(&value)) { std::ostringstream out; out << std::setprecision(15) << *number; return out.str(); }
        if (const auto* flag = std::get_if<bool>(&value)) return *flag ? "true" : "false";
        std::string joined;
        for (const auto& item : std::get<std::vector<std::string>>(value)) {
            if (!joined.empty()) joined += ", ";
            joined += item;
        }
        return joined;
    }
    void clear() { value = std::string(); }
};
using Vars = std::unordered_map<std::string, VariableValue>;
using Types = std::unordered_map<std::string, std::string>;

struct Flow {
    bool broke = false;
    bool continued = false;
    bool stopped = false;
};

struct EveryTask {
    std::vector<std::string> body;
    std::chrono::milliseconds interval{1000};
    std::chrono::steady_clock::time_point nextRun;
    bool manualMode = false;
    bool uiMode = false;
};

struct CollisionWatch { std::string first; std::string second; std::string result; };

struct UserFunction {
    std::string name;
    std::vector<std::string> params;
    std::vector<std::string> body;
};

struct UiBlock {
    std::string name;
    std::string group;
    std::string color;
    std::string shape = "square";
    std::string glowColor = "";
    double glowIntensity = 0;
    int z = 0;
    double x = 0;
    double y = 0;
    double width = 100;
    double height = 100;
    double cornerRadius = 0;
    double opacity = 1.0;
    bool visible = true;
};

struct UiSprite {
    std::string name;
    std::string group;
    std::string path;
    double x = 0;
    double y = 0;
    double width = 10;
    double height = 10;
    double opacity = 1.0;
    bool visible = true;
};

struct StructField { std::string type; std::string initial; };
using StructDefinition = std::unordered_map<std::string, StructField>;

struct UiAnimation {
    std::string name;
    double startX = 0;
    double startY = 0;
    double dx = 0;
    double dy = 0;
    double startTime = 0;
    double duration = 0;
    int fps = 60;
};

struct UiButton {
    std::string name;
    std::string group;
    std::string label;
    double x = 0;
    double y = 0;
    double width = 20;
    double height = 10;
    std::string color = "#7043BE";
    std::string textColor = "#FFFFFF";
    std::string font = "sans";
    double opacity = 1.0;
    double cornerRadius = 7;
    double textSize = 0.34;
    bool textBold = true;
    bool visible = true;
    std::vector<std::string> body;
};

struct UiText {
    std::string name;
    std::string group;
    std::string value;
    std::string color;
    std::string font = "sans";
    std::string align = "left";
    double x = 0;
    double y = 0;
    double size = 5;
    double opacity = 1.0;
    bool bold = false;
    bool visible = true;
};

struct UiGroup { std::string name; bool visible = false; std::string parent = "default"; };

using Defaults = std::unordered_map<std::string, std::string>;

std::unordered_map<std::string, UserFunction> userFunctions;
std::vector<UiBlock> uiBlocks;
std::vector<UiSprite> uiSprites;
std::vector<UiAnimation> uiAnimations;
std::vector<EveryTask> everyTasks;
std::vector<CollisionWatch> collisionWatches;
std::vector<UiButton> uiButtons;
std::vector<UiText> uiTexts;
std::vector<UiGroup> uiGroups;
std::string currentUiGroup = "default";
std::unordered_map<std::string, StructDefinition> userStructs;
std::vector<std::string> uiProgram;
std::filesystem::path projectRoot = std::filesystem::current_path();
std::filesystem::path currentSourcePath;
bool filesEnabled = false;
bool jsonEnabled = false;
std::vector<std::string> groupStack;
std::vector<std::unordered_set<std::string>> groupSnapshots;
std::unordered_map<std::string, std::vector<std::string>> variableGroups;
std::unordered_set<std::string> executedFileKeys;
std::unordered_set<std::string> activeRunFiles;
Vars uiVariables;
Types uiTypes;
Defaults uiDefaults;
std::unordered_map<std::string, int> keyboardControls;
std::unordered_map<std::string, unsigned> controllerControls;
bool controlsEnabled = false;
bool webEnabled = false;
bool collisionEnabled = false;
std::string webPageHtml;
std::vector<std::string> readProgram(std::istream& source);
std::size_t errorCount = 0;
std::chrono::steady_clock::time_point uiStart;
std::mt19937 trgRandomEngine(std::random_device{}());
bool uiEntityBounds(const std::string& name, double& x, double& y, double& width, double& height);
void refreshCollisionWatches(Vars& variables, Types& types);

// ---------------------------------------------------------
// New features: particles, viewport, network, shapes, glows
// ---------------------------------------------------------
struct Particle {
    std::string name;
    double x = 0, y = 0;
    double vx = 0, vy = 0;
    double life = 0;
    double maxLife = 0;
    std::string color = "#FFFFFF";
    double size = 4;
    bool active = true;
};

std::vector<Particle> particles;
double viewportX = 0;
double viewportY = 0;
bool networkEnabled = false;
bool powershellEnabled = false;

struct HttpRequest {
    std::string method;
    std::string url;
    std::string body;
    std::string resultVar;
};

std::vector<HttpRequest> pendingHttpRequests;

std::string httpGet(const std::string& url, std::string& error);
std::string httpPost(const std::string& url, const std::string& body, std::string& error);

bool uiGroupIsVisible(const std::string& name) {
    if (name.empty() || name == "default") return true;
    const auto group = std::find_if(uiGroups.begin(), uiGroups.end(), [&](const UiGroup& item) {
        return item.name == name;
    });
    return group != uiGroups.end() && group->visible && uiGroupIsVisible(group->parent);
}

bool uiNameIsVisible(const std::string& name) {
    for (const UiBlock& item : uiBlocks) if (item.name == name) return item.visible && uiGroupIsVisible(item.group);
    for (const UiSprite& item : uiSprites) if (item.name == name) return item.visible && uiGroupIsVisible(item.group);
    for (const UiButton& item : uiButtons) if (item.name == name) return item.visible && uiGroupIsVisible(item.group);
    for (const UiText& item : uiTexts) if (item.name == name) return item.visible && uiGroupIsVisible(item.group);
    return false;
}

#ifdef __ANDROID__
extern std::string androidOutput;
extern jobject trgActivity;
extern std::unordered_map<std::string, std::string> androidMobileInputs;
extern std::atomic<bool> androidStopRequested;
extern std::mutex androidInputMutex;
#if defined(TRG_BOUNCYBALLS_EXTENSION)
extern std::deque<std::array<double, 3>> androidGamePointerEvents;
extern std::mutex androidGamePointerMutex;
std::unique_ptr<bouncyballs::GameCore> trgBouncyGame;
AAssetManager* trgBouncyAssets = nullptr;
int trgBouncyViewportWidth = 1080;
int trgBouncyViewportHeight = 1920;
void ensureTrgBouncyGame();
void trgSetBouncyAssets(AAssetManager* assets) { trgBouncyAssets = assets; }
void trgSetBouncyViewport(int width, int height) {
    trgBouncyViewportWidth = std::max(1, width);
    trgBouncyViewportHeight = std::max(1, height);
    if (trgBouncyGame) trgBouncyGame->setViewportSize(trgBouncyViewportWidth, trgBouncyViewportHeight);
}
void trgConfigureBouncyGame(bool developerUnlocked, bool fullTrg, bool editorUnlocked, bool tester, bool debugMode) {
    ensureTrgBouncyGame();
    if (!trgBouncyGame) return;
    trgBouncyGame->setDeveloperUnlocked(developerUnlocked);
    trgBouncyGame->setFullTrgEnabled(fullTrg);
    trgBouncyGame->setEditorUnlocked(editorUnlocked);
    trgBouncyGame->setTester(tester);
    trgBouncyGame->setDebugMode(debugMode);
}
void ensureTrgBouncyGame() {
    if (trgBouncyGame) return;
    trgBouncyGame = std::make_unique<bouncyballs::GameCore>();
    trgBouncyGame->setViewportSize(trgBouncyViewportWidth, trgBouncyViewportHeight);
    trgBouncyGame->setAssetLoader([](const std::string& path) {
        if (!trgBouncyAssets) return std::string{};
        AAsset* asset = AAssetManager_open(trgBouncyAssets, path.c_str(), AASSET_MODE_BUFFER);
        if (!asset) return std::string{};
        const off_t length = AAsset_getLength(asset);
        std::string result(static_cast<std::size_t>(length), '\0');
        if (length > 0) AAsset_read(asset, result.data(), static_cast<size_t>(length));
        AAsset_close(asset);
        return result;
    });
}
#endif
std::string trgAskAndroid(const std::string& prompt, bool& accepted);
void trgStudioActionAndroid(const std::string& action);
#endif

#ifdef _WIN32
HWND mainWindow = nullptr;
HWND inputControl = nullptr;
HFONT openButtonFont = nullptr;
HFONT codeEditorFont = nullptr;
bool inputFinished = false;
bool inputAccepted = false;
bool loadingEditorText = false;
bool editorDirty = false;
bool previewMode = false;
std::string inputResult;
constexpr int openButtonId = 101;
constexpr int inputEditId = 201;
constexpr int inputButtonId = 202;
constexpr int saveButtonId = 102;
constexpr int runButtonId = 103;
constexpr int previewButtonId = 104;
constexpr int newButtonId = 105;
constexpr int codeEditorId = 301;
HWND openButton = nullptr;
HWND saveButton = nullptr;
HWND runButton = nullptr;
HWND previewButton = nullptr;
HWND newButton = nullptr;
HWND codeEditor = nullptr;
std::wstring inputPrompt;
std::wstring outputBuffer;
ULONG_PTR gdiplusToken = 0;
std::unordered_map<std::string, std::unique_ptr<Gdiplus::Image>> desktopSpriteCache;

bool runSourceFile(const std::filesystem::path& path);
bool loadSourceFile(const std::filesystem::path& path);
bool saveSourceFile();
bool runEditorSource();

int windowDpi(HWND window) {
    using GetDpiForWindowFunction = UINT (WINAPI*)(HWND);
    static const auto getDpiForWindow = reinterpret_cast<GetDpiForWindowFunction>(
        GetProcAddress(GetModuleHandleW(L"user32.dll"), "GetDpiForWindow"));
    return getDpiForWindow ? static_cast<int>(getDpiForWindow(window)) : 96;
}

int scaleForDpi(HWND window, int value) {
    return MulDiv(value, windowDpi(window), 96);
}

int systemDpi() {
    using GetDpiForSystemFunction = UINT (WINAPI*)();
    static const auto getDpiForSystem = reinterpret_cast<GetDpiForSystemFunction>(
        GetProcAddress(GetModuleHandleW(L"user32.dll"), "GetDpiForSystem"));
    if (getDpiForSystem) return static_cast<int>(getDpiForSystem());
    HDC screen = GetDC(nullptr);
    if (!screen) return 96;
    const int dpi = GetDeviceCaps(screen, LOGPIXELSX);
    ReleaseDC(nullptr, screen);
    return dpi > 0 ? dpi : 96;
}

void enablePerMonitorDpiAwareness() {
    using SetProcessDpiAwarenessContextFunction = BOOL (WINAPI*)(HANDLE);
    static const auto setAwareness = reinterpret_cast<SetProcessDpiAwarenessContextFunction>(
        GetProcAddress(GetModuleHandleW(L"user32.dll"), "SetProcessDpiAwarenessContext"));
    if (setAwareness && setAwareness(reinterpret_cast<HANDLE>(static_cast<INT_PTR>(-4)))) return;
    SetProcessDPIAware();
}

void setOpenButtonFont(HWND window) {
    if (openButtonFont) DeleteObject(openButtonFont);
    openButtonFont = CreateFontW(scaleForDpi(window, 14), 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
    const HWND buttons[] = {openButton, saveButton, runButton, previewButton, newButton};
    if (openButtonFont) for (HWND button : buttons) if (button)
        SendMessageW(button, WM_SETFONT, reinterpret_cast<WPARAM>(openButtonFont), TRUE);
    if (codeEditorFont) DeleteObject(codeEditorFont);
    codeEditorFont = CreateFontW(-scaleForDpi(window, 16), 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        FIXED_PITCH | FF_MODERN, L"Consolas");
    if (codeEditor && codeEditorFont) SendMessageW(codeEditor, WM_SETFONT,
        reinterpret_cast<WPARAM>(codeEditorFont), TRUE);
}

bool openSourceDialog(std::filesystem::path& path, HWND owner) {
    wchar_t fileName[32768] = {};
    OPENFILENAMEW dialog = {};
    dialog.lStructSize = sizeof(dialog);
    dialog.hwndOwner = owner;
    dialog.lpstrFilter = L"TRG files (*.trg)\0*.trg\0All files (*.*)\0*.*\0";
    dialog.lpstrFile = fileName;
    dialog.nMaxFile = static_cast<DWORD>(sizeof(fileName) / sizeof(fileName[0]));
    dialog.lpstrDefExt = L"trg";
    dialog.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
    if (!GetOpenFileNameW(&dialog)) return false;
    path = std::filesystem::path(fileName);
    return true;
}

bool saveSourceDialog(std::filesystem::path& path, HWND owner) {
    wchar_t fileName[32768] = {};
    if (!path.empty()) {
        std::wcsncpy(fileName, path.c_str(), 32767);
        fileName[32767] = L'\0';
    }
    OPENFILENAMEW dialog = {};
    dialog.lStructSize = sizeof(dialog);
    dialog.hwndOwner = owner;
    dialog.lpstrFilter = L"TRG files (*.trg)\0*.trg\0Text files (*.txt)\0*.txt\0All files (*.*)\0*.*\0";
    dialog.lpstrFile = fileName;
    dialog.nMaxFile = static_cast<DWORD>(sizeof(fileName) / sizeof(fileName[0]));
    dialog.lpstrDefExt = L"trg";
    dialog.Flags = OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;
    if (!GetSaveFileNameW(&dialog)) return false;
    path = std::filesystem::path(fileName);
    return true;
}

void layoutOutputControls(HWND window) {
    RECT client = {};
    GetClientRect(window, &client);
    const LONG width = std::max<LONG>(0, client.right - client.left);
    const LONG height = std::max<LONG>(0, client.bottom - client.top);
    const LONG headerHeight = std::min<LONG>(scaleForDpi(window, 76),
        std::max<LONG>(scaleForDpi(window, 48), height / 5));
    const LONG gap = scaleForDpi(window, 5);
    const LONG margin = scaleForDpi(window, 12);
    const LONG desiredWidth = scaleForDpi(window, 88);
    const LONG available = std::max<LONG>(scaleForDpi(window, 5 * 56 + 4 * 5), width - scaleForDpi(window, 205) - margin * 2);
    const LONG buttonWidth = std::clamp<LONG>((available - gap * 4) / 5,
        scaleForDpi(window, 56), desiredWidth);
    const LONG buttonHeight = scaleForDpi(window, 34);
    const HWND buttons[] = {openButton, saveButton, runButton, previewButton, newButton};
    LONG x = width - margin - (buttonWidth + gap) * 5 + gap;
    const LONG y = std::max<LONG>(0, (headerHeight - buttonHeight) / 2);
    for (HWND button : buttons) {
        if (button) MoveWindow(button, x, y, buttonWidth, buttonHeight, TRUE);
        x += buttonWidth + gap;
    }
    if (codeEditor) {
        const LONG editorMargin = scaleForDpi(window, 10);
        const LONG top = headerHeight + editorMargin;
        const LONG outputMinimum = scaleForDpi(window, 80);
        const LONG canvasMinimum = scaleForDpi(window, 100);
        const LONG panelMinimum = std::min(height, headerHeight + canvasMinimum);
        const LONG panelMaximum = std::max(panelMinimum, height - outputMinimum);
        const LONG panelTop = std::clamp<LONG>(height * 65 / 100, panelMinimum, panelMaximum);
        const LONG bottom = std::max<LONG>(top + 1, panelTop - editorMargin);
        MoveWindow(codeEditor, editorMargin, top, std::max<LONG>(1, width - editorMargin * 2), bottom - top, TRUE);
        ShowWindow(codeEditor, previewMode ? SW_HIDE : SW_SHOW);
    }
}

std::wstring wideFromUtf8(const std::string& value) {
    if (value.empty()) return std::wstring();
    const int size = MultiByteToWideChar(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), nullptr, 0);
    if (size <= 0) return std::wstring(value.begin(), value.end());
    std::wstring result(static_cast<std::size_t>(size), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), &result[0], size);
    return result;
}

std::string utf8FromWide(const std::wstring& value) {
    if (value.empty()) return std::string();
    const int size = WideCharToMultiByte(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), nullptr, 0, nullptr, nullptr);
    if (size <= 0) return std::string(value.begin(), value.end());
    std::string result(static_cast<std::size_t>(size), '\0');
    WideCharToMultiByte(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), &result[0], size, nullptr, nullptr);
    return result;
}

void updateEditorWindowTitle() {
    if (!mainWindow) return;
    std::wstring title = L"TRG Editor - ";
    title += currentSourcePath.empty() ? L"Untitled.trg" : currentSourcePath.filename().wstring();
    if (editorDirty) title += L" *";
    SetWindowTextW(mainWindow, title.c_str());
}

bool confirmDiscardEditorChanges(HWND owner) {
    if (!editorDirty) return true;
    return MessageBoxW(owner, L"Discard unsaved changes to this TRG file?", L"Unsaved TRG code",
                       MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2) == IDYES;
}

void appendOutput(const std::string& text) {
#ifdef TRG_TEST_CONSOLE
    std::cout << text;
#else
    outputBuffer += wideFromUtf8(text);
    if (outputBuffer.size() > 20000) {
        const std::size_t cutoff = outputBuffer.find(L'\n', outputBuffer.size() - 15000);
        if (cutoff != std::wstring::npos) outputBuffer.erase(0, cutoff + 1);
    }
    if (mainWindow) InvalidateRect(mainWindow, nullptr, FALSE);
#endif
}
#else
void appendOutput(const std::string& text) {
#ifdef __ANDROID__
    androidOutput += text;
#elif defined(TRG_WEB)
    trgWebOutput += text;
#else
    std::cout << text;
#endif
}
#endif




std::string trim(const std::string& s) {
    size_t first = s.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, last - first + 1);
}

std::string htmlEscape(const std::string& value) {
    std::string result;
    for (char c : value) {
        if (c == '&') result += "&amp;";
        else if (c == '<') result += "&lt;";
        else if (c == '>') result += "&gt;";
        else if (c == '"') result += "&quot;";
        else if (c == '\'') result += "&#39;";
        else result += c;
    }
    return result;
}

std::string base64Encode(const std::string& input) {
    static const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string output;
    unsigned value = 0;
    int bits = -6;
    for (unsigned char byte : input) {
        value = (value << 8) | byte;
        bits += 8;
        while (bits >= 0) {
            output.push_back(alphabet[(value >> bits) & 0x3f]);
            bits -= 6;
        }
    }
    if (bits > -6) output.push_back(alphabet[((value << 8) >> (bits + 8)) & 0x3f]);
    while (output.size() % 4) output.push_back('=');
    return output;
}

std::string lower(std::string s) {
    for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

std::string suggestedCommand(const std::string& input) {
    static const std::vector<std::string> commands = {
        "say", "ask", "word", "num", "toggle", "color", "if", "else", "loop", "every", "random",
        "group", "define", "stop", "exit", "break", "continue", "add", "text",
        "button", "block", "animate", "collides", "touching", "move_ui", "register", "move", "subtract", "multiply",
        "watch_collision",
        "divide", "write_file", "append_file", "read_file", "delete_file", "file_exists",
        "upper", "lower", "length", "contains", "replace", "push", "get", "remove", "set", "increase", "decrease",
        "play_audio", "show_image", "play_video", "run_wasm", "web_page", "web_heading",
        "web_text", "web_color", "web_button", "web_image", "web_show"
    };
    const std::string word = lower(input);
    std::size_t bestDistance = word.size();
    std::string best;
    for (const std::string& candidate : commands) {
        std::vector<std::size_t> row(candidate.size() + 1);
        for (std::size_t j = 0; j <= candidate.size(); ++j) row[j] = j;
        for (std::size_t i = 1; i <= word.size(); ++i) {
            std::size_t diagonal = row[0];
            row[0] = i;
            for (std::size_t j = 1; j <= candidate.size(); ++j) {
                const std::size_t previous = row[j];
                row[j] = std::min({row[j] + 1, row[j - 1] + 1,
                                   diagonal + (word[i - 1] == candidate[j - 1] ? 0u : 1u)});
                diagonal = previous;
            }
        }
        if (row.back() < bestDistance) { bestDistance = row.back(); best = candidate; }
    }
    const std::size_t threshold = std::max<std::size_t>(2, word.size() / 3);
    return bestDistance <= threshold ? best : std::string();
}

std::string stripComment(const std::string& line) {
    bool inQuotes = false;
    for (std::size_t i = 0; i + 1 < line.size(); ++i) {
        if (line[i] == '\\' && inQuotes) { ++i; continue; }
        if (line[i] == '"') inQuotes = !inQuotes;
        else if (!inQuotes && line[i] == '/' && line[i + 1] == '/') return line.substr(0, i);
    }
    return line;
}

bool validName(const std::string& name) {
    if (name.empty()) return false;
    if (!(std::isalpha(static_cast<unsigned char>(name[0])) || name[0] == '_')) return false;
    for (char c : name) {
        if (!(std::isalnum(static_cast<unsigned char>(c)) || c == '_')) return false;
    }
    return true;
}

bool validVariablePath(const std::string& name) {
    const std::size_t dot = name.find('.');
    return dot == std::string::npos ? validName(name) :
        name.find('.', dot + 1) == std::string::npos &&
        validName(name.substr(0, dot)) && validName(name.substr(dot + 1));
}

void reportError(std::size_t lineNumber, const std::string& reason, const std::string& part = "") {
    std::string msg = "line " + std::to_string(lineNumber) + ": " + reason;
    if (!part.empty()) msg += " \"" + part + "\"";
    ++errorCount;
    appendOutput("ERROR " + msg + "\r\n");
}

bool findBlock(const std::vector<std::string>& program, std::size_t start,
               std::size_t end, std::size_t& open, std::size_t& close) {
    
    
    std::size_t depth = 0;
    open = std::string::npos;
    bool opened = false;
    for (close = start; close < end; ++close) {
        std::string line = program[close];
        std::size_t firstCharacter = 0;
        if (close == start && !line.empty()) {
            std::size_t lastCharacter = line.size();
            while (lastCharacter > 0 && std::isspace(static_cast<unsigned char>(line[lastCharacter - 1]))) --lastCharacter;
            if (lastCharacter > 0 && line[lastCharacter - 1] == '(') {
                open = close;
                depth = 1;
                opened = true;
                firstCharacter = lastCharacter;
            } else {
                const std::string header = lower(trim(line));
                if (header == "if" || header.rfind("if ", 0) == 0 ||
                    header == "else" || header.rfind("else ", 0) == 0 ||
                    header == "loop" || header.rfind("loop ", 0) == 0 ||
                    header == "every" || header.rfind("every ", 0) == 0 ||
                    header == "group" || header.rfind("group ", 0) == 0 ||
                    header == "record" || header.rfind("record ", 0) == 0 ||
                    header.rfind("struct ", 0) == 0 || header.rfind("class ", 0) == 0 ||
                    header == "define" || header.rfind("define ", 0) == 0 ||
                    header == "add" || header.rfind("add ", 0) == 0) {
                    firstCharacter = line.size();
                }
            }
        }
        for (std::size_t i = firstCharacter; i < line.size(); ++i) {
            if (line[i] == '"') {
                ++i;
                while (i < line.size() && (line[i] != '"' || (i > 0 && line[i - 1] == '\\'))) ++i;
                continue;
            }
            if (line[i] == '/' && i + 1 < line.size() && line[i + 1] == '/') break;
            if (line[i] == '(') {
                if (!opened) {
                    opened = true;
                    open = close;
                    depth = 1;
                } else {
                    ++depth;
                }
            } else if (line[i] == ')' && opened) {
                if (--depth == 0) return true;
            }
        }
    }
    return false;
}

Defaults standardDefaults() {
    return {{"num", "0"}, {"word", "none"}, {"color", "#222222"}, {"toggle", "false"}};
}

std::vector<std::string> tokenize(const std::string& text) {
    std::vector<std::string> result;
    std::string token;
    bool quoted = false;
    char quote = '\0';
    for (std::size_t i = 0; i < text.size(); ++i) {
        const char c = text[i];
        if (quoted) {
            if (c == '\\' && i + 1 < text.size()) {
                token += text[++i];
            } else if (c == quote) {
                quoted = false;
            } else {
                token += c;
            }
        } else if (c == '"' || c == '\'') {
            quoted = true;
            quote = c;
        } else if (std::isspace(static_cast<unsigned char>(c)) || c == ',') {
            if (!token.empty()) {
                result.push_back(token);
                token.clear();
            }
        } else {
            token += c;
        }
    }
    if (!token.empty()) result.push_back(token);
    return result;
}

std::string valueFor(const std::string& word, const Vars& variables) {
    const auto it = variables.find(word);
    return it == variables.end() ? word : static_cast<std::string>(it->second);
}




std::size_t positionAfterArgument(const std::string& line, const std::string& token) {
    const std::size_t commandEnd = line.find_first_of(" \t");
    if (commandEnd == std::string::npos) return std::string::npos;
    const std::size_t position = line.find(token, commandEnd + 1);
    return position == std::string::npos ? std::string::npos : position + token.size();
}

std::string unbraced(const std::string& value) {
    if (value.size() >= 2 && value.front() == '{' && value.back() == '}') {
        return value.substr(1, value.size() - 2);
    }
    return value;
}

bool parseNumber(const std::string& text, double& value) {
    std::istringstream input(text);
    input >> value;
    return input && (input >> std::ws).eof();
}

bool parseDurationMilliseconds(const std::string& source, long long& milliseconds) {
    std::string value = lower(trim(source));
    bool useMilliseconds = false;
    if (value.size() >= 2 && value.substr(value.size() - 2) == "ms") {
        useMilliseconds = true;
        value.resize(value.size() - 2);
    } else if (!value.empty() && value.back() == 's') {
        value.pop_back();
    }
    double duration = 0;
    if (!parseNumber(value, duration) || duration < 0) return false;
    const double result = useMilliseconds ? duration : duration * 1000.0;
    if (result > static_cast<double>(std::numeric_limits<long long>::max())) return false;
    milliseconds = static_cast<long long>(result);
    return true;
}

std::filesystem::path projectFilePath(const std::string& name, bool& valid) {
    const std::filesystem::path root = std::filesystem::absolute(projectRoot).lexically_normal();
    const std::filesystem::path requested = std::filesystem::u8path(name);
    
    
    if (name.empty() || requested.is_absolute() || requested.has_root_name() || requested.has_root_directory()) {
        valid = false;
        return {};
    }
    const std::filesystem::path target = (root / requested).lexically_normal();
    const std::filesystem::path relative = target.lexically_relative(root);
    valid = !relative.empty() && relative != "." && *relative.begin() != "..";
    return target;
}

std::filesystem::path canonicalProjectPath(const std::filesystem::path& path) {
    std::error_code error;
    const std::filesystem::path canonical = std::filesystem::weakly_canonical(path, error);
    return error ? std::filesystem::absolute(path).lexically_normal() : canonical;
}



std::filesystem::path projectExistingFile(const std::string& name, bool& valid) {
    const std::filesystem::path path = projectFilePath(name, valid);
    if (!valid) return {};
    std::error_code error;
    const std::filesystem::path canonical = std::filesystem::weakly_canonical(path, error);
    if (error) { valid = false; return {}; }
    const std::filesystem::path root = std::filesystem::weakly_canonical(projectRoot, error);
    if (error) { valid = false; return {}; }
    const std::filesystem::path relative = canonical.lexically_relative(root);
    valid = !relative.empty() && relative != "." && *relative.begin() != "..";
    return canonical;
}

std::string numberString(double value) {
    std::ostringstream output;
    output << std::setprecision(15) << value;
    return output.str();
}

bool parseColor(const std::string& color, unsigned& red, unsigned& green, unsigned& blue) {
    std::string hex = color;
    const bool blueRedGreenFormat = hex.size() >= 2 && hex[0] == '#' && hex[1] == '#';
    if (blueRedGreenFormat) hex.erase(0, 2);
    else if (!hex.empty() && hex[0] == '#') hex.erase(0, 1);
    else if (hex.size() >= 8 && hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X')) {
        hex.erase(0, 2);
        if (hex.size() == 8) hex.erase(0, 2);
    }
    else return false;
    if (hex.size() != 6 || !std::all_of(hex.begin(), hex.end(), [](unsigned char c) { return std::isxdigit(c) != 0; })) return false;
    try {
        if (blueRedGreenFormat) {
            blue = static_cast<unsigned>(std::stoul(hex.substr(0, 2), nullptr, 16));
            red = static_cast<unsigned>(std::stoul(hex.substr(2, 2), nullptr, 16));
            green = static_cast<unsigned>(std::stoul(hex.substr(4, 2), nullptr, 16));
        } else {
            red = static_cast<unsigned>(std::stoul(hex.substr(0, 2), nullptr, 16));
            green = static_cast<unsigned>(std::stoul(hex.substr(2, 2), nullptr, 16));
            blue = static_cast<unsigned>(std::stoul(hex.substr(4, 2), nullptr, 16));
        }
        return true;
    } catch (const std::exception&) {
        return false;
    }
}




bool evaluateExpression(const std::string& expr, Vars& variables, double& result) {
    std::string t = trim(expr);
    if (t.empty()) return false;
    std::vector<std::string> tokens;
    for (std::size_t i = 0; i < t.size();) {
        if (std::isspace(static_cast<unsigned char>(t[i]))) { ++i; continue; }
        if (std::string("()+-*/%^,").find(t[i]) != std::string::npos) {
            tokens.emplace_back(1, t[i++]);
            continue;
        }
        const std::size_t start = i;
        while (i < t.size() && !std::isspace(static_cast<unsigned char>(t[i])) &&
               std::string("()+-*/%^,").find(t[i]) == std::string::npos) ++i;
        tokens.push_back(t.substr(start, i - start));
    }
    std::size_t position = 0;
    std::function<bool(double&)> parseSum, parseProduct, parseUnary, parsePower, parsePrimary;
    parsePrimary = [&](double& value) {
        if (position >= tokens.size()) return false;
        const std::string token = tokens[position++];
        if (token == "(") {
            if (!parseSum(value) || position >= tokens.size() || tokens[position++] != ")") return false;
            return true;
        }
        if (lower(token) == "sqrt") {
            const bool parenthesized = position < tokens.size() && tokens[position] == "(";
            if (parenthesized) {
                ++position;
                if (!parseSum(value) || position >= tokens.size() || tokens[position++] != ")") return false;
            } else if (!parseUnary(value)) return false;
            if (value < 0) return false;
            value = std::sqrt(value);
            return true;
        }
        const auto variable = variables.find(token);
        return parseNumber(variable == variables.end() ? token : static_cast<std::string>(variable->second), value);
    };
    parsePower = [&](double& value) {
        if (!parsePrimary(value)) return false;
        if (position < tokens.size() && tokens[position] == "^") {
            ++position;
            double exponent = 0;
            if (!parseUnary(exponent)) return false;
            value = std::pow(value, exponent);
        }
        return std::isfinite(value);
    };
    parseUnary = [&](double& value) {
        if (position < tokens.size() && (tokens[position] == "+" || tokens[position] == "-")) {
            const bool negative = tokens[position++] == "-";
            if (!parseUnary(value)) return false;
            if (negative) value = -value;
            return true;
        }
        return parsePower(value);
    };
    parseProduct = [&](double& value) {
        if (!parseUnary(value)) return false;
        while (position < tokens.size() && (tokens[position] == "*" || tokens[position] == "/" || tokens[position] == "%")) {
            const std::string op = tokens[position++];
            double right = 0;
            if (!parseUnary(right) || ((op == "/" || op == "%") && right == 0)) return false;
            if (op == "*") value *= right;
            else if (op == "/") value /= right;
            else value = std::fmod(value, right);
        }
        return std::isfinite(value);
    };
    parseSum = [&](double& value) {
        if (!parseProduct(value)) return false;
        while (position < tokens.size() && (tokens[position] == "+" || tokens[position] == "-")) {
            const std::string op = tokens[position++];
            double right = 0;
            if (!parseProduct(right)) return false;
            value = op == "+" ? value + right : value - right;
        }
        return std::isfinite(value);
    };
    return parseSum(result) && position == tokens.size();
}

bool conditionIsTrue(const std::string& cond, Vars& variables) {
#if defined(_WIN32) && !defined(TRG_TEST_CONSOLE)
    if (controlsEnabled) {
        for (const auto& control : keyboardControls) {
            variables[control.first] = (GetAsyncKeyState(control.second) & 0x8000) ? "press" : "released";
        }

        struct ControllerState {
            unsigned long packetNumber;
            unsigned short buttons;
            unsigned char leftTrigger;
            unsigned char rightTrigger;
            short thumbLX;
            short thumbLY;
            short thumbRX;
            short thumbRY;
        };
        using GetControllerState = unsigned long (WINAPI*)(unsigned long, ControllerState*);
        static HMODULE controllerModule = nullptr;
        static GetControllerState getControllerState = nullptr;
        static bool controllerLookupDone = false;
        if (!controllerLookupDone) {
            controllerLookupDone = true;
            const wchar_t* modules[] = {L"xinput1_4.dll", L"xinput1_3.dll", L"xinput9_1_0.dll"};
            for (const wchar_t* module : modules) {
                controllerModule = LoadLibraryW(module);
                if (controllerModule) {
                    const FARPROC procedure = GetProcAddress(controllerModule, "XInputGetState");
                    static_assert(sizeof(procedure) == sizeof(getControllerState), "Windows function pointers must have equal size");
                    std::memcpy(&getControllerState, &procedure, sizeof(procedure));
                    if (getControllerState) break;
                    FreeLibrary(controllerModule);
                    controllerModule = nullptr;
                }
            }
        }
        ControllerState state = {};
        const bool connected = getControllerState && getControllerState(0, &state) == 0;
        for (const auto& control : controllerControls) {
            bool pressed = false;
            if (connected) {
                if (control.second == 0x10000) pressed = state.leftTrigger >= 30;
                else if (control.second == 0x20000) pressed = state.rightTrigger >= 30;
                else pressed = (state.buttons & control.second) != 0;
            }
            variables[control.first] = pressed ? "press" : "released";
        }
        if (connected) {
            variables["controller_left_stick_x"] = std::clamp(state.thumbLX / 32767.0, -1.0, 1.0);
            variables["controller_left_stick_y"] = std::clamp(state.thumbLY / 32767.0, -1.0, 1.0);
            variables["controller_right_stick_x"] = std::clamp(state.thumbRX / 32767.0, -1.0, 1.0);
            variables["controller_right_stick_y"] = std::clamp(state.thumbRY / 32767.0, -1.0, 1.0);
        }
    }
#elif defined(__ANDROID__)
    if (controlsEnabled) { std::lock_guard<std::mutex> lock(androidInputMutex); for (const auto& control : androidMobileInputs) variables[control.first] = control.second; }
#elif defined(TRG_WEB)
    if (controlsEnabled) for (const auto& control : keyboardControls) variables[control.first] = trgWebPressedKeys.count(control.first) ? "press" : "released";
#endif
    std::string t = trim(cond);
    const auto variable = variables.find(t);
    if (variable != variables.end()) t = static_cast<std::string>(variable->second);
    if (lower(t) == "true") return true;
    if (lower(t) == "false") return false;

    const std::vector<std::string> operators = {"==", "!=", "<=", ">=", "=", "<", ">"};
    for (const std::string& op : operators) {
        const std::size_t pos = t.find(op);
        if (pos == std::string::npos) continue;
        const std::string leftText = trim(t.substr(0, pos));
        const std::string rightText = trim(t.substr(pos + op.size()));
        double left = 0, right = 0;
        if (evaluateExpression(leftText, variables, left) && evaluateExpression(rightText, variables, right)) {
            if (op == "==") return left == right;
            if (op == "!=") return left != right;
            if (op == "<=") return left <= right;
            if (op == ">=") return left >= right;
            if (op == "=") return left == right;
            if (op == "<") return left < right;
            return left > right;
        }
        const std::string leftValue = valueFor(leftText, variables);
        const std::string rightValue = valueFor(rightText, variables);
        return (op == "==" || op == "=") ? leftValue == rightValue :
            op == "!=" ? leftValue != rightValue : false;
    }
    double numeric = 0;
    if (parseNumber(t, numeric)) return numeric != 0;
    return false;
}

bool askForInput(const std::string& prompt, std::string& answer);

void initializeControls(Vars& variables, Types& types) {
    controlsEnabled = true;
    keyboardControls.clear();
    controllerControls.clear();
    const std::pair<const char*, int> keys[] = {
        {"key_a", 'A'}, {"key_b", 'B'}, {"key_c", 'C'}, {"key_d", 'D'},
        {"key_e", 'E'}, {"key_f", 'F'}, {"key_g", 'G'}, {"key_h", 'H'},
        {"key_i", 'I'}, {"key_j", 'J'}, {"key_k", 'K'}, {"key_l", 'L'},
        {"key_m", 'M'}, {"key_n", 'N'}, {"key_o", 'O'}, {"key_p", 'P'},
        {"key_q", 'Q'}, {"key_r", 'R'}, {"key_s", 'S'}, {"key_t", 'T'},
        {"key_u", 'U'}, {"key_v", 'V'}, {"key_w", 'W'}, {"key_x", 'X'},
        {"key_y", 'Y'}, {"key_z", 'Z'}, {"key_0", '0'}, {"key_1", '1'},
        {"key_2", '2'}, {"key_3", '3'}, {"key_4", '4'}, {"key_5", '5'},
        {"key_6", '6'}, {"key_7", '7'}, {"key_8", '8'}, {"key_9", '9'},
        {"key_space", VK_SPACE}, {"key_enter", VK_RETURN}, {"key_escape", VK_ESCAPE},
        {"key_tab", VK_TAB}, {"key_shift", VK_SHIFT}, {"key_control", VK_CONTROL},
        {"key_alt", VK_MENU}, {"key_left", VK_LEFT}, {"key_right", VK_RIGHT},
        {"key_up", VK_UP}, {"key_down", VK_DOWN}
    };
    for (const auto& key : keys) {
        keyboardControls[key.first] = key.second;
        variables[key.first] = "released";
        types[key.first] = "word";
    }
    const std::pair<const char*, unsigned> buttons[] = {
        {"controller_a", 0x1000}, {"controller_b", 0x2000},
        {"controller_x", 0x4000}, {"controller_y", 0x8000},
        {"controller_lb", 0x0100}, {"controller_rb", 0x0200},
        {"controller_lt", 0x10000}, {"controller_rt", 0x20000},
        {"controller_left_stick", 0x0040}, {"controller_right_stick", 0x0080},
        {"controller_start", 0x0010}, {"controller_back", 0x0020},
        {"controller_dpad_up", 0x0001}, {"controller_dpad_down", 0x0002},
        {"controller_dpad_left", 0x0004}, {"controller_dpad_right", 0x0008}
    };
    for (const auto& button : buttons) {
        controllerControls[button.first] = button.second;
        variables[button.first] = "released";
        types[button.first] = "word";
    }
    for (int functionKey = 1; functionKey <= 24; ++functionKey) {
        const std::string name = "key_f" + std::to_string(functionKey);
        keyboardControls[name] = VK_F1 + functionKey - 1;
        variables[name] = "released";
        types[name] = "word";
    }
    for (int digit = 0; digit <= 9; ++digit) {
        const std::string name = "key_numpad" + std::to_string(digit);
        keyboardControls[name] = VK_NUMPAD0 + digit;
        variables[name] = "released";
        types[name] = "word";
    }
    const std::pair<const char*, int> extraKeys[] = {
        {"key_backspace", VK_BACK}, {"key_delete", VK_DELETE}, {"key_insert", VK_INSERT},
        {"key_home", VK_HOME}, {"key_end", VK_END}, {"key_page_up", VK_PRIOR},
        {"key_page_down", VK_NEXT}, {"key_caps_lock", VK_CAPITAL},
        {"key_num_lock", VK_NUMLOCK}, {"key_scroll_lock", VK_SCROLL},
        {"key_print_screen", VK_SNAPSHOT}, {"key_pause", VK_PAUSE},
        {"key_left_windows", VK_LWIN}, {"key_right_windows", VK_RWIN},
        {"key_apps", VK_APPS}, {"key_numpad_add", VK_ADD},
        {"key_numpad_subtract", VK_SUBTRACT}, {"key_numpad_multiply", VK_MULTIPLY},
        {"key_numpad_divide", VK_DIVIDE}, {"key_numpad_decimal", VK_DECIMAL},
        {"key_semicolon", VK_OEM_1}, {"key_equals", VK_OEM_PLUS},
        {"key_comma", VK_OEM_COMMA}, {"key_minus", VK_OEM_MINUS},
        {"key_period", VK_OEM_PERIOD}, {"key_slash", VK_OEM_2},
        {"key_backtick", VK_OEM_3}, {"key_left_bracket", VK_OEM_4},
        {"key_backslash", VK_OEM_5}, {"key_right_bracket", VK_OEM_6},
        {"key_apostrophe", VK_OEM_7}
    };
    for (const auto& key : extraKeys) {
        keyboardControls[key.first] = key.second;
        variables[key.first] = "released";
        types[key.first] = "word";
    }
    const char* mobileControls[] = {
        "mobile_tap", "mobile_hold", "mobile_swipe_left", "mobile_swipe_right",
        "mobile_swipe_up", "mobile_swipe_down", "mobile_button_1", "mobile_button_2",
        "mobile_button_3", "mobile_button_4", "mobile_button_5", "mobile_button_6",
        "mobile_button_7", "mobile_button_8"
    };
    for (const char* control : mobileControls) {
#ifdef __ANDROID__
        variables[control] = "released";
#else
        variables[control] = "unavailable";
#endif
        types[control] = "word";
    }
#ifdef __ANDROID__
    for (const auto& key : keyboardControls) variables[key.first] = "unavailable";
    for (const auto& button : controllerControls) variables[button.first] = "unavailable";
    {
        std::lock_guard<std::mutex> lock(androidInputMutex);
        androidMobileInputs.clear();
        for (const char* control : mobileControls) androidMobileInputs[control] = "released";
    }
#endif
    const char* controllerAxes[] = {
        "controller_left_stick_x", "controller_left_stick_y",
        "controller_right_stick_x", "controller_right_stick_y"
    };
    for (const char* axis : controllerAxes) {
        variables[axis] = 0.0;
        types[axis] = "num";
    }
}




std::string jsonEscape(const std::string& text) {
    std::string out;
    out.reserve(text.size() + 8);
    for (unsigned char c : text) {
        switch (c) {
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\b': out += "\\b"; break;
            case '\f': out += "\\f"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:
                if (c < 0x20) {
                    char buffer[8];
                    std::snprintf(buffer, sizeof(buffer), "\\u%04x", static_cast<unsigned>(c));
                    out += buffer;
                } else {
                    out += static_cast<char>(c);
                }
        }
    }
    return out;
}

struct JsonNode {
    enum Kind { Null, Number, Boolean, String, Array, Object } kind = Null;
    double number = 0;
    bool boolean = false;
    std::string text;
    std::vector<JsonNode> items;
    std::vector<std::pair<std::string, JsonNode>> members;
};

void appendUtf8(std::string& out, unsigned codePoint) {
    if (codePoint <= 0x7F) out += static_cast<char>(codePoint);
    else if (codePoint <= 0x7FF) {
        out += static_cast<char>(0xC0 | (codePoint >> 6));
        out += static_cast<char>(0x80 | (codePoint & 0x3F));
    } else if (codePoint <= 0xFFFF) {
        out += static_cast<char>(0xE0 | (codePoint >> 12));
        out += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
        out += static_cast<char>(0x80 | (codePoint & 0x3F));
    } else {
        out += static_cast<char>(0xF0 | (codePoint >> 18));
        out += static_cast<char>(0x80 | ((codePoint >> 12) & 0x3F));
        out += static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F));
        out += static_cast<char>(0x80 | (codePoint & 0x3F));
    }
}

class JsonParser {
public:
    explicit JsonParser(const std::string& source) : source(source) {}
    bool parse(JsonNode& result) {
        skipSpace();
        if (!parseValue(result)) return false;
        skipSpace();
        return position == source.size();
    }
private:
    const std::string& source;
    std::size_t position = 0;

    void skipSpace() {
        while (position < source.size() && std::isspace(static_cast<unsigned char>(source[position]))) ++position;
    }
    bool parseValue(JsonNode& result) {
        skipSpace();
        if (position >= source.size()) return false;
        const char c = source[position];
        if (c == '{') return parseObject(result);
        if (c == '[') return parseArray(result);
        if (c == '"') {
            result.kind = JsonNode::String;
            return parseString(result.text);
        }
        if (source.compare(position, 4, "true") == 0) {
            position += 4; result.kind = JsonNode::Boolean; result.boolean = true; return true;
        }
        if (source.compare(position, 5, "false") == 0) {
            position += 5; result.kind = JsonNode::Boolean; result.boolean = false; return true;
        }
        if (source.compare(position, 4, "null") == 0) {
            position += 4; result.kind = JsonNode::Null; return true;
        }
        return parseNumber(result);
    }
    bool parseString(std::string& result) {
        if (position >= source.size() || source[position] != '"') return false;
        ++position;
        result.clear();
        while (position < source.size()) {
            const char c = source[position++];
            if (c == '"') return true;
            if (c != '\\') { result += c; continue; }
            if (position >= source.size()) return false;
            const char escape = source[position++];
            switch (escape) {
                case '"': result += '"'; break;
                case '\\': result += '\\'; break;
                case '/': result += '/'; break;
                case 'b': result += '\b'; break;
                case 'f': result += '\f'; break;
                case 'n': result += '\n'; break;
                case 'r': result += '\r'; break;
                case 't': result += '\t'; break;
                case 'u': {
                    if (position + 4 > source.size()) return false;
                    unsigned codePoint = 0;
                    for (int digit = 0; digit < 4; ++digit) {
                        const char hex = source[position++];
                        codePoint <<= 4;
                        if (hex >= '0' && hex <= '9') codePoint |= static_cast<unsigned>(hex - '0');
                        else if (hex >= 'a' && hex <= 'f') codePoint |= static_cast<unsigned>(hex - 'a' + 10);
                        else if (hex >= 'A' && hex <= 'F') codePoint |= static_cast<unsigned>(hex - 'A' + 10);
                        else return false;
                    }
                    appendUtf8(result, codePoint);
                    break;
                }
                default: return false;
            }
        }
        return false;
    }
    bool parseNumber(JsonNode& result) {
        const std::size_t start = position;
        if (position < source.size() && source[position] == '-') ++position;
        while (position < source.size() && std::isdigit(static_cast<unsigned char>(source[position]))) ++position;
        if (position < source.size() && source[position] == '.') {
            ++position;
            while (position < source.size() && std::isdigit(static_cast<unsigned char>(source[position]))) ++position;
        }
        if (position < source.size() && (source[position] == 'e' || source[position] == 'E')) {
            ++position;
            if (position < source.size() && (source[position] == '+' || source[position] == '-')) ++position;
            while (position < source.size() && std::isdigit(static_cast<unsigned char>(source[position]))) ++position;
        }
        if (position == start) return false;
        const std::string number = source.substr(start, position - start);
        char* end = nullptr;
        const double value = std::strtod(number.c_str(), &end);
        if (!end || *end != '\0') return false;
        result.kind = JsonNode::Number;
        result.number = value;
        return true;
    }
    bool parseArray(JsonNode& result) {
        ++position;
        result.kind = JsonNode::Array;
        skipSpace();
        if (position < source.size() && source[position] == ']') { ++position; return true; }
        while (position < source.size()) {
            JsonNode item;
            if (!parseValue(item)) return false;
            result.items.push_back(std::move(item));
            skipSpace();
            if (position < source.size() && source[position] == ',') { ++position; continue; }
            if (position < source.size() && source[position] == ']') { ++position; return true; }
            return false;
        }
        return false;
    }
    bool parseObject(JsonNode& result) {
        ++position;
        result.kind = JsonNode::Object;
        skipSpace();
        if (position < source.size() && source[position] == '}') { ++position; return true; }
        while (position < source.size()) {
            skipSpace();
            std::string key;
            if (!parseString(key)) return false;
            skipSpace();
            if (position >= source.size() || source[position] != ':') return false;
            ++position;
            JsonNode value;
            if (!parseValue(value)) return false;
            result.members.emplace_back(std::move(key), std::move(value));
            skipSpace();
            if (position < source.size() && source[position] == ',') { ++position; continue; }
            if (position < source.size() && source[position] == '}') { ++position; return true; }
            return false;
        }
        return false;
    }
};

std::string jsonNodeCompact(const JsonNode& node) {
    switch (node.kind) {
        case JsonNode::Null: return "null";
        case JsonNode::Number: return numberString(node.number);
        case JsonNode::Boolean: return node.boolean ? "true" : "false";
        case JsonNode::String: return "\"" + jsonEscape(node.text) + "\"";
        case JsonNode::Array: {
            std::string out = "[";
            for (std::size_t i = 0; i < node.items.size(); ++i) {
                if (i) out += ',';
                out += jsonNodeCompact(node.items[i]);
            }
            out += ']';
            return out;
        }
        case JsonNode::Object: {
            std::string out = "{";
            for (std::size_t i = 0; i < node.members.size(); ++i) {
                if (i) out += ',';
                out += "\"" + jsonEscape(node.members[i].first) + "\":" + jsonNodeCompact(node.members[i].second);
            }
            out += '}';
            return out;
        }
    }
    return "null";
}

std::string jsonValueText(const VariableValue& value) {
    if (const auto* number = std::get_if<double>(&value.value)) return numberString(*number);
    if (const auto* flag = std::get_if<bool>(&value.value)) return *flag ? "true" : "false";
    if (const auto* text = std::get_if<std::string>(&value.value)) return "\"" + jsonEscape(*text) + "\"";
    const auto& items = std::get<std::vector<std::string>>(value.value);
    std::string out = "[";
    for (std::size_t i = 0; i < items.size(); ++i) {
        if (i) out += ',';
        out += "\"" + jsonEscape(items[i]) + "\"";
    }
    out += ']';
    return out;
}

std::string serializeJsonObject(const std::vector<std::pair<std::string, VariableValue>>& entries, bool clean) {
    std::vector<std::pair<std::string, VariableValue>> sorted = entries;
    
    std::sort(sorted.begin(), sorted.end(), [](const auto& left, const auto& right) {
        if (left.first.size() != right.first.size()) return left.first.size() > right.first.size();
        return left.first < right.first;
    });
    if (sorted.empty()) return "{}";
    std::string out;
    if (clean) {
        out = "{\n";
        for (std::size_t i = 0; i < sorted.size(); ++i) {
            out += "  \"" + jsonEscape(sorted[i].first) + "\": " + jsonValueText(sorted[i].second);
            if (i + 1 < sorted.size()) out += ',';
            out += '\n';
        }
        out += '}';
    } else {
        out = "{";
        for (std::size_t i = 0; i < sorted.size(); ++i) {
            if (i) out += ',';
            out += "\"" + jsonEscape(sorted[i].first) + "\":" + jsonValueText(sorted[i].second);
        }
        out += '}';
    }
    return out;
}

void flattenJson(const JsonNode& node, const std::string& name, Vars& variables, Types& types) {
    if (node.kind == JsonNode::Object) {
        for (const auto& member : node.members) {
            const std::string key = name.empty() ? member.first : name + "." + member.first;
            flattenJson(member.second, key, variables, types);
        }
        return;
    }
    if (name.empty()) return;
    switch (node.kind) {
        case JsonNode::Number:
            variables[name] = node.number;
            types[name] = "num";
            break;
        case JsonNode::Boolean:
            variables[name] = node.boolean;
            types[name] = "toggle";
            break;
        case JsonNode::Null:
            variables[name] = std::string();
            types[name] = "word";
            break;
        case JsonNode::String:
            variables[name] = node.text;
            types[name] = "word";
            break;
        case JsonNode::Array: {
            std::vector<std::string> items;
            for (const auto& item : node.items) {
                if (item.kind == JsonNode::String) items.push_back(item.text);
                else items.push_back(jsonNodeCompact(item));
            }
            variables[name] = std::move(items);
            types[name] = "list";
            break;
        }
        default:
            break;
    }
}

bool readJsonIntoVariables(const std::filesystem::path& path, Vars& variables, Types& types, std::string& error) {
    std::ifstream input(path, std::ios::binary);
    if (!input) { error = "could not read JSON file"; return false; }
    std::ostringstream content;
    content << input.rdbuf();
    std::string source = content.str();
    if (source.size() >= 3 && static_cast<unsigned char>(source[0]) == 0xEF &&
        static_cast<unsigned char>(source[1]) == 0xBB && static_cast<unsigned char>(source[2]) == 0xBF)
        source.erase(0, 3);
    JsonNode root;
    JsonParser parser(source);
    if (!parser.parse(root)) { error = "invalid JSON"; return false; }
    flattenJson(root, "", variables, types);
    return true;
}




// ---------------------------------------------------------
// HTTP / network implementation (add (network))
// ---------------------------------------------------------
#ifdef _WIN32
std::string httpGet(const std::string& url, std::string& error) {
    error.clear();
    std::string result;
    std::string scheme, host, path;
    std::size_t schemeEnd = url.find("://");
    if (schemeEnd != std::string::npos) {
        scheme = url.substr(0, schemeEnd);
        std::size_t pathStart = url.find('/', schemeEnd + 3);
        if (pathStart != std::string::npos) {
            host = url.substr(schemeEnd + 3, pathStart - schemeEnd - 3);
            path = url.substr(pathStart);
        } else {
            host = url.substr(schemeEnd + 3);
            path = "/";
        }
    } else {
        error = "invalid URL";
        return {};
    }

    HINTERNET session = WinHttpOpen(L"TRG/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                    WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!session) { error = "could not open WinHTTP session"; return {}; }

    std::wstring whost = wideFromUtf8(host);
    std::wstring wpath = wideFromUtf8(path);
    INTERNET_PORT port = (lower(scheme) == "https") ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT;
    DWORD flag = (lower(scheme) == "https") ? WINHTTP_FLAG_SECURE : 0;

    HINTERNET connect = WinHttpConnect(session, whost.c_str(), port, 0);
    if (!connect) { WinHttpCloseHandle(session); error = "could not connect"; return {}; }

    HINTERNET request = WinHttpOpenRequest(connect, L"GET", wpath.c_str(),
                                           nullptr, WINHTTP_NO_REFERER,
                                           WINHTTP_DEFAULT_ACCEPT_TYPES, flag);
    if (!request) { WinHttpCloseHandle(connect); WinHttpCloseHandle(session); error = "could not open request"; return {}; }

    if (!WinHttpSendRequest(request, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                            WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
        DWORD err = GetLastError();
        WinHttpCloseHandle(request); WinHttpCloseHandle(connect); WinHttpCloseHandle(session);
        error = "send request failed: " + std::to_string(err);
        return {};
    }
    if (!WinHttpReceiveResponse(request, nullptr)) {
        DWORD err = GetLastError();
        WinHttpCloseHandle(request); WinHttpCloseHandle(connect); WinHttpCloseHandle(session);
        error = "receive response failed: " + std::to_string(err);
        return {};
    }

    DWORD size = 0;
    do {
        size = 0;
        if (!WinHttpQueryDataAvailable(request, &size)) break;
        if (size == 0) break;
        std::vector<char> buffer(size + 1);
        DWORD read = 0;
        if (!WinHttpReadData(request, buffer.data(), size, &read)) break;
        buffer[read] = '\0';
        result += buffer.data();
    } while (size > 0);

    WinHttpCloseHandle(request);
    WinHttpCloseHandle(connect);
    WinHttpCloseHandle(session);
    return result;
}
#else
std::string httpGet(const std::string& url, std::string& error) { error = "network only on Windows"; return {}; }
#endif

#ifdef _WIN32
std::string httpPost(const std::string& url, const std::string& body, std::string& error) {
    error.clear();
    std::string result;
    std::string scheme, host, path;
    std::size_t schemeEnd = url.find("://");
    if (schemeEnd != std::string::npos) {
        scheme = url.substr(0, schemeEnd);
        std::size_t pathStart = url.find('/', schemeEnd + 3);
        if (pathStart != std::string::npos) {
            host = url.substr(schemeEnd + 3, pathStart - schemeEnd - 3);
            path = url.substr(pathStart);
        } else {
            host = url.substr(schemeEnd + 3);
            path = "/";
        }
    } else {
        error = "invalid URL";
        return {};
    }

    HINTERNET session = WinHttpOpen(L"TRG/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                    WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!session) { error = "could not open WinHTTP session"; return {}; }

    std::wstring whost = wideFromUtf8(host);
    std::wstring wpath = wideFromUtf8(path);
    INTERNET_PORT port = (lower(scheme) == "https") ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT;
    DWORD flag = (lower(scheme) == "https") ? WINHTTP_FLAG_SECURE : 0;

    HINTERNET connect = WinHttpConnect(session, whost.c_str(), port, 0);
    if (!connect) { WinHttpCloseHandle(session); error = "could not connect"; return {}; }

    HINTERNET request = WinHttpOpenRequest(connect, L"POST", wpath.c_str(),
                                           nullptr, WINHTTP_NO_REFERER,
                                           WINHTTP_DEFAULT_ACCEPT_TYPES, flag);
    if (!request) { WinHttpCloseHandle(connect); WinHttpCloseHandle(session); error = "could not open request"; return {}; }

    std::string headers = "Content-Type: application/x-www-form-urlencoded\r\n";
    std::wstring wheaders = wideFromUtf8(headers);

    if (!WinHttpSendRequest(request, wheaders.c_str(), (DWORD)wheaders.size(),
                            (LPVOID)body.data(), (DWORD)body.size(), (DWORD)body.size(), 0)) {
        DWORD err = GetLastError();
        WinHttpCloseHandle(request); WinHttpCloseHandle(connect); WinHttpCloseHandle(session);
        error = "send request failed: " + std::to_string(err);
        return {};
    }
    if (!WinHttpReceiveResponse(request, nullptr)) {
        DWORD err = GetLastError();
        WinHttpCloseHandle(request); WinHttpCloseHandle(connect); WinHttpCloseHandle(session);
        error = "receive response failed: " + std::to_string(err);
        return {};
    }

    DWORD size = 0;
    do {
        size = 0;
        if (!WinHttpQueryDataAvailable(request, &size)) break;
        if (size == 0) break;
        std::vector<char> buffer(size + 1);
        DWORD read = 0;
        if (!WinHttpReadData(request, buffer.data(), size, &read)) break;
        buffer[read] = '\0';
        result += buffer.data();
    } while (size > 0);

    WinHttpCloseHandle(request);
    WinHttpCloseHandle(connect);
    WinHttpCloseHandle(session);
    return result;
}
#else
std::string httpPost(const std::string&, const std::string&, std::string& error) { error = "HTTP POST is not available in this build"; return {}; }
#endif

// ---------------------------------------------------------

// PowerShell library (add (powershell))
// ---------------------------------------------------------
bool psIsSafeCommand(const std::string& cmd) {
    // Whitelist of safe PowerShell commands/verbs-nouns
    static const std::unordered_set<std::string> safeCommands = {
        "Write-Host", "Write-Verbose", "Write-Debug",
        "Get-Help", "Get-Member", "Get-Command",
        "Set-Variable", "Get-Variable", "New-Variable",
        "Clear-Variable", "Remove-Variable",
        "Select-Object", "Where-Object",
        "Measure-Object", "Sort-Object",
        "Export-Csv", "ConvertTo-Csv",
        "Get-Process", "Get-Service",
        "Start-Process", "Stop-Process",
        "Test-Path", "Get-ChildItem",
        "Write-Output", "Read-Host"
    };
    // Check if cmd starts with any safe verb-noun
    for (const auto& safe : safeCommands) {
        if (cmd.rfind(safe, 0) == 0) return true;
    }
    return false;
}

std::string psExecuteSafe(const std::string& code, std::string& error) {
    error.clear();
    // Simple whitelist-based approach: only allow known safe commands
    // Extract first token (command name)
    std::string firstLine = code;
    std::size_t nl = firstLine.find('\n');
    if (nl != std::string::npos) firstLine = firstLine.substr(0, nl);
    firstLine = trim(firstLine);
    if (firstLine.empty()) return "";
    
    std::istringstream iss(firstLine);
    std::string cmd;
    iss >> cmd;
    
    if (!psIsSafeCommand(cmd)) {
        error = "powershell command not in whitelist: " + cmd;
        return {};
    }
    
    // For whitelisted commands, we'll just log/append output what would run
    // In a real implementation, you'd use PowerShell API or invoke with constraints
    // Here we just append the command to output as a safe placeholder
    return "POWERSHELL EXECUTED (whitelisted): " + code;
}

struct LibraryState {
    bool files = false;
    bool json = false;
    bool web = false;
    bool collision = false;
    bool manual = false;
    bool ui = false;
    bool controls = false;
    bool network = false;
};

std::string parenContent(const std::string& line) {
    
    
    bool quoted = false;
    char quote = '\0';
    std::size_t open = std::string::npos;
    int depth = 0;
    for (std::size_t i = 0; i < line.size(); ++i) {
        const char c = line[i];
        if (quoted) {
            if (c == '\\' && i + 1 < line.size()) ++i;
            else if (c == quote) quoted = false;
            continue;
        }
        if (c == '"' || c == '\'') { quoted = true; quote = c; continue; }
        if (c == '(') {
            if (open == std::string::npos) { open = i; depth = 1; }
            else ++depth;
        } else if (c == ')' && open != std::string::npos) {
            if (--depth == 0) return trim(line.substr(open + 1, i - open - 1));
        }
    }
    return "";
}

bool addDirectiveKind(const std::vector<std::string>& tokens, std::string& kind) {
    if (tokens.size() < 2 || lower(tokens[0]) != "add") return false;
    const std::string second = lower(tokens[1]);
    if (second == "global" || second == "(global)") { kind = "global"; return true; }
    if (second == "connect" || second == "(connect)") { kind = "connect"; return true; }
    return false;
}



void stripProjectDirectives(const std::vector<std::string>& program,
                            std::vector<std::string>& kept,
                            std::vector<std::string>& globals,
                            std::vector<std::string>& connects,
                            bool report,
                            std::unordered_set<std::string>* seenGlobals = nullptr) {
    for (std::size_t i = 0; i < program.size(); ++i) {
        const std::string line = trim(program[i]);
        const std::vector<std::string> tokens = tokenize(line);
        std::string kind;
        if (!addDirectiveKind(tokens, kind)) {
            kept.push_back(program[i]);
            continue;
        }
        if (kind == "global") {
            std::size_t open = 0, close = 0;
            if (findBlock(program, i, program.size(), open, close)) {
                std::string signature;
                for (std::size_t row = open + 1; row < close; ++row) signature += program[row] + '\n';
                if (seenGlobals && !seenGlobals->insert(signature).second) {
                    i = close;
                    continue;
                }
                for (std::size_t row = open + 1; row < close; ++row) globals.push_back(program[row]);
                i = close;
            } else if (report) {
                reportError(i + 1, "add global needs a ( ... ) block");
            }
            continue;
        }
        std::string target;
        if (lower(tokens[1]) == "(connect)") {
            if (tokens.size() >= 3) target = tokens[2];
        } else {
            target = parenContent(line);
        }
        if (target.empty() && tokens.size() >= 3) target = tokens[2];
        if (target.size() >= 2 && target.front() == '(' && target.back() == ')')
            target = target.substr(1, target.size() - 2);
        if (!target.empty()) connects.push_back(target);
        else if (report) reportError(i + 1, "add connect needs a file name");
    }
}

void scanLibraries(const std::vector<std::string>& program, LibraryState& libraries) {
    for (const auto& raw : program) {
        const std::vector<std::string> tokens = tokenize(trim(raw));
        if (tokens.size() < 2 || lower(tokens[0]) != "add") continue;
        const std::string second = lower(tokens[1]);
        if (second == "global" || second == "(global)" || second == "connect" || second == "(connect)") continue;
        if (second.size() < 3 || second.front() != '(' || second.back() != ')') continue;
        const std::string name = lower(second.substr(1, second.size() - 2));
        if (name == "all") {
            libraries.files = libraries.json = libraries.web = libraries.collision = true;
            libraries.manual = libraries.ui = libraries.controls = libraries.network = true;
        } else if (name == "files") libraries.files = true;
        else if (name == "json") libraries.json = true;
        else if (name == "web") libraries.web = true;
        else if (name == "collision") libraries.collision = true;
        else if (name == "manual") libraries.manual = true;
        else if (name == "ui" || name == "animation") libraries.ui = true;
        else if (name == "controls") libraries.controls = true;
        else if (name == "network") libraries.network = true;
    }
}

void collectTrgFiles(const std::filesystem::path& root, std::vector<std::filesystem::path>& files) {
    std::error_code error;
    const std::filesystem::path canonicalRoot = std::filesystem::weakly_canonical(root, error);
    std::filesystem::recursive_directory_iterator iterator(root,
        std::filesystem::directory_options::skip_permission_denied, error);
    const std::filesystem::recursive_directory_iterator end;
    while (!error && iterator != end) {
        std::error_code entryError;
        if (iterator->is_regular_file(entryError) && !entryError &&
            lower(iterator->path().extension().u8string()) == ".trg") {
            
            std::error_code canonicalError;
            const std::filesystem::path canonical =
                std::filesystem::weakly_canonical(iterator->path(), canonicalError);
            bool inside = !canonicalError;
            if (inside) {
                const std::filesystem::path relative = canonical.lexically_relative(canonicalRoot);
                inside = !relative.empty() && relative != "." && *relative.begin() != "..";
            }
            if (inside) files.push_back(iterator->path());
        }
        iterator.increment(error);
    }
    std::sort(files.begin(), files.end());
}





std::vector<std::string> mergePrograms(const std::vector<std::string>& mainProgram,
                                       const std::filesystem::path& mainPath,
                                       bool includeProjectGlobals, bool report,
                                       const std::filesystem::path& excludePath = {}) {
    struct ProjectNode {
        std::filesystem::path path;
        std::vector<std::string> program;
        std::vector<std::string> connects;
    };

    std::vector<std::string> globals;
    std::vector<std::string> keptMain;
    std::vector<std::string> mainConnects;
    std::unordered_set<std::string> seenGlobals;
    stripProjectDirectives(mainProgram, keptMain, globals, mainConnects, report, &seenGlobals);

    const std::filesystem::path mainCanonical = mainPath.empty() ? std::filesystem::path() : canonicalProjectPath(mainPath);
    const std::string mainKey = mainCanonical.empty() ? "__main__" : lower(mainCanonical.u8string());

    std::unordered_map<std::string, ProjectNode> nodes;
    std::vector<std::filesystem::path> files;
    collectTrgFiles(projectRoot, files);
    for (const auto& file : files) {
        const std::filesystem::path canonical = canonicalProjectPath(file);
        const std::string key = lower(canonical.u8string());
        if (key == mainKey) continue;
        std::ifstream input(file, std::ios::binary);
        if (!input) continue;
        std::vector<std::string> kept, localGlobals, connects;
        stripProjectDirectives(readProgram(input), kept, localGlobals, connects, false, &seenGlobals);
        if (includeProjectGlobals) globals.insert(globals.end(), localGlobals.begin(), localGlobals.end());
        nodes[key] = {canonical, std::move(kept), std::move(connects)};
    }
    nodes[mainKey] = {mainCanonical, keptMain, mainConnects};

    std::unordered_map<std::string, std::vector<std::string>> adjacency;
    std::unordered_map<std::string, std::vector<std::string>> connectionErrors;
    std::unordered_set<std::string> processed;
    std::deque<std::string> pending;
    for (const auto& item : nodes) pending.push_back(item.first);

    auto loadConnectedNode = [&](const std::string& raw, const std::string& owner) -> std::string {
        bool valid = false;
        const std::filesystem::path path = projectExistingFile(raw, valid);
        if (!valid) {
            connectionErrors[owner].push_back("connect target must stay inside the TRG project folder: \"" + raw + "\"");
            return {};
        }
        const std::filesystem::path canonical = canonicalProjectPath(path);
        const std::string key = lower(canonical.u8string());
        if (nodes.count(key)) return key;
        std::ifstream input(canonical, std::ios::binary);
        if (!input) {
            connectionErrors[owner].push_back("could not open connected file: \"" + canonical.u8string() + "\"");
            return {};
        }
        std::vector<std::string> kept, localGlobals, connects;
        stripProjectDirectives(readProgram(input), kept, localGlobals, connects, false, &seenGlobals);
        if (includeProjectGlobals) globals.insert(globals.end(), localGlobals.begin(), localGlobals.end());
        nodes[key] = {canonical, std::move(kept), std::move(connects)};
        return key;
    };

    while (!pending.empty()) {
        const std::string key = pending.front();
        pending.pop_front();
        if (!processed.insert(key).second) continue;
        for (const auto& connect : nodes[key].connects) {
            const std::string target = loadConnectedNode(connect, key);
            if (target.empty() || target == key) continue;
            adjacency[key].push_back(target);
            adjacency[target].push_back(key);
            if (!processed.count(target)) pending.push_back(target);
        }
    }

    
    std::vector<std::string> order;
    std::unordered_set<std::string> visited;
    std::deque<std::string> queue;
    queue.push_back(mainKey);
    visited.insert(mainKey);
    while (!queue.empty()) {
        const std::string key = queue.front();
        queue.pop_front();
        order.push_back(key);
        const auto edges = adjacency.find(key);
        if (edges == adjacency.end()) continue;
        for (const auto& next : edges->second) {
            if (visited.insert(next).second) queue.push_back(next);
        }
    }

    if (report) {
        for (const auto& key : order) {
            const auto errors = connectionErrors.find(key);
            if (errors == connectionErrors.end()) continue;
            for (const auto& message : errors->second) {
                ++errorCount;
                appendOutput("ERROR " + message + "\r\n");
            }
        }
    }

    std::string excludeKey;
    if (!excludePath.empty()) excludeKey = lower(canonicalProjectPath(excludePath).u8string());

    std::vector<std::string> merged;
    merged.insert(merged.end(), globals.begin(), globals.end());
    for (auto it = order.rbegin(); it != order.rend(); ++it) {
        if (*it == mainKey || *it == excludeKey) continue;
        if (executedFileKeys.count(*it)) continue;
        const auto node = nodes.find(*it);
        if (node == nodes.end()) continue;
        merged.insert(merged.end(), node->second.program.begin(), node->second.program.end());
    }
    merged.insert(merged.end(), keptMain.begin(), keptMain.end());
    for (const auto& key : order) executedFileKeys.insert(key);
    return merged;
}




void registerFunctions(const std::vector<std::string>& program, bool clearFirst = true) {
    if (clearFirst) userFunctions.clear();
    for (std::size_t i = 0; i < program.size(); ++i) {
        std::string line = trim(program[i]);
        if (lower(line).rfind("define ", 0) != 0) continue;

        std::istringstream in(line);
        std::string kw, name;
        in >> kw >> name;
        if (!validName(name)) continue;

        std::vector<std::string> params;
        std::string p;
        while (in >> p) {
            if (p == "(") break; 
            if (p != ",") params.push_back(p);
        }

        std::size_t open, close;
        if (findBlock(program, i, program.size(), open, close)) {
            userFunctions[lower(name)] = {name, params,
                std::vector<std::string>(program.begin() + static_cast<std::ptrdiff_t>(open + 1),
                                         program.begin() + static_cast<std::ptrdiff_t>(close))};
        }
    }
}

void executeBlock(const std::vector<std::string>& program, std::size_t begin, std::size_t end,
                  Vars& variables, Types& types, Defaults& defaults, Flow& flow,
                  bool manualMode = false, bool uiMode = false) {
    for (std::size_t i = begin; i < end; ++i) {
#ifdef __ANDROID__
        if (androidStopRequested.load()) {
            flow.stopped = true;
            return;
        }
#endif
        if (flow.stopped || flow.broke || flow.continued) return;
        
        std::string line = trim(program[i]);
        if (line.empty() || line == "(" || line == ")") continue;
        if (line == "{" || line == "}") {
            reportError(i + 1, "TRG blocks use ( and ); braces are not supported");
            continue;
        }
        
        const std::vector<std::string> tokens = tokenize(line);
        if (tokens.empty()) continue;
        const std::string cmd = lower(tokens[0]);

#if defined(__ANDROID__) && defined(TRG_BOUNCYBALLS_EXTENSION)
        
        
        if (cmd == "game") {
            ensureTrgBouncyGame();
            if (!trgBouncyGame || tokens.size() < 2) {
                reportError(i + 1, "use game start|update|pointer|menu|retry|next|screen|overlay|query|object|set|purchase|equip");
                continue;
            }
            const std::string action = lower(tokens[1]);
            auto readNumber = [&](std::size_t index, double& value) {
                return index < tokens.size() && evaluateExpression(tokens[index], variables, value);
            };
            if (action == "start" && tokens.size() == 3) {
                double level = 0;
                if (!readNumber(2, level) || level < 1) reportError(i + 1, "game start needs a positive level number");
                else trgBouncyGame->startLevel(static_cast<int>(level));
            } else if (action == "update" && tokens.size() == 3) {
                double seconds = 0;
                if (!readNumber(2, seconds) || seconds < 0) reportError(i + 1, "game update needs elapsed seconds");
                else trgBouncyGame->update(static_cast<float>(std::min(seconds, 0.25)));
            } else if (action == "pointer" && tokens.size() == 2) {
                std::deque<std::array<double, 3>> events;
                { std::lock_guard<std::mutex> lock(androidGamePointerMutex); events.swap(androidGamePointerEvents); }
                for (const auto& event : events) {
                    const float x = static_cast<float>(event[1] * trgBouncyViewportWidth / 100.0);
                    const float y = static_cast<float>(event[2] * trgBouncyViewportHeight / 100.0);
                    if (event[0] == 0) trgBouncyGame->onTouchDown(x, y);
                    else if (event[0] == 1) trgBouncyGame->onTouchMove(x, y);
                    else trgBouncyGame->onTouchUp(x, y);
                }
            } else if (action == "menu" && tokens.size() == 2) {
                trgBouncyGame->goToMainMenu();
            } else if (action == "dev_auth" && tokens.size() == 2) {
                trgBouncyGame->requestDeveloperPassword();
            } else if (action == "request_editor" && tokens.size() == 2) {
                trgBouncyGame->requestTrgEditor();
            } else if (action == "retry" && tokens.size() == 2) {
                trgBouncyGame->retryLevel();
            } else if (action == "next" && tokens.size() == 2) {
                trgBouncyGame->nextLevel();
            } else if (action == "screen" && tokens.size() == 3) {
                const std::string screen = lower(tokens[2]);
                if (screen == "menu") trgBouncyGame->setScreen(bouncyballs::ScreenState::MAIN_MENU);
                else if (screen == "gameplay" || screen == "game") trgBouncyGame->setScreen(bouncyballs::ScreenState::GAMEPLAY);
                else if (screen == "shop") trgBouncyGame->setScreen(bouncyballs::ScreenState::SHOP);
                else if (screen == "extra") trgBouncyGame->setScreen(bouncyballs::ScreenState::EXTRA);
                else if (screen == "editor") trgBouncyGame->setScreen(bouncyballs::ScreenState::EDITOR);
                else reportError(i + 1, "game screen must be menu, gameplay, shop, extra, or editor", screen);
            } else if (action == "overlay" && tokens.size() == 3) {
                const std::string overlay = lower(tokens[2]);
                if (overlay == "none") trgBouncyGame->setOverlay(bouncyballs::OverlayState::NONE);
                else if (overlay == "levels" || overlay == "level_picker") trgBouncyGame->setOverlay(bouncyballs::OverlayState::LEVEL_PICKER);
                else if (overlay == "settings") trgBouncyGame->setOverlay(bouncyballs::OverlayState::SETTINGS);
                else if (overlay == "developer" || overlay == "dev_menu") trgBouncyGame->setOverlay(bouncyballs::OverlayState::DEV_MENU);
                else if (overlay == "win") trgBouncyGame->setOverlay(bouncyballs::OverlayState::GAME_WIN);
                else if (overlay == "loss") trgBouncyGame->setOverlay(bouncyballs::OverlayState::GAME_OVER);
                else reportError(i + 1, "unknown game overlay", overlay);
            } else if (action == "query" && tokens.size() == 4 && validName(tokens[3])) {
                const auto& player = trgBouncyGame->player().state();
                const std::string property = lower(tokens[2]);
                if (property == "x") variables[tokens[3]] = static_cast<double>(player.x);
                else if (property == "y") variables[tokens[3]] = static_cast<double>(player.y);
                else if (property == "vx") variables[tokens[3]] = static_cast<double>(player.vx);
                else if (property == "vy") variables[tokens[3]] = static_cast<double>(player.vy);
                else if (property == "moving") variables[tokens[3]] = player.isUserMoving;
                else if (property == "lost") variables[tokens[3]] = player.isLost;
                else if (property == "active") variables[tokens[3]] = player.isActive;
                else if (property == "lives") variables[tokens[3]] = static_cast<double>(player.lives);
                else if (property == "bounce_count") variables[tokens[3]] = static_cast<double>(player.bounceCount);
                else if (property == "dragging") variables[tokens[3]] = trgBouncyGame->isDragging();
                else if (property == "touch_x") variables[tokens[3]] = static_cast<double>(trgBouncyGame->currentTouchX());
                else if (property == "touch_y") variables[tokens[3]] = static_cast<double>(trgBouncyGame->currentTouchY());
                else if (property == "level") variables[tokens[3]] = static_cast<double>(trgBouncyGame->currentLevel());
                else if (property == "coins") variables[tokens[3]] = static_cast<double>(trgBouncyGame->coins());
                else if (property == "object_count") variables[tokens[3]] = static_cast<double>(trgBouncyGame->physicsWorld().objects.size());
                else if (property == "dark_mode") variables[tokens[3]] = trgBouncyGame->darkMode();
                else if (property == "sound_muted") variables[tokens[3]] = trgBouncyGame->soundMuted();
                else if (property == "level_locked") variables[tokens[3]] = trgBouncyGame->levelLocked();
                else if (property == "editor_unlocked") variables[tokens[3]] = trgBouncyGame->isEditorUnlocked();
                else if (property == "developer_unlocked") variables[tokens[3]] = trgBouncyGame->isDeveloperUnlocked();
                else if (property == "full_trg") variables[tokens[3]] = trgBouncyGame->fullTrgEnabled();
                else if (property == "equipped_skin") variables[tokens[3]] = trgBouncyGame->equippedSkin();
                else if (property == "equipped_color") variables[tokens[3]] = trgBouncyGame->equippedColor();
                else if (property == "equipped_design") variables[tokens[3]] = trgBouncyGame->equippedDesign();
                else if (property == "equipped_special") variables[tokens[3]] = trgBouncyGame->equippedSpecial();
                else if (property == "overlay") {
                    const auto overlay = trgBouncyGame->currentOverlay();
                    variables[tokens[3]] = overlay == bouncyballs::OverlayState::GAME_WIN ? "win" :
                        overlay == bouncyballs::OverlayState::GAME_OVER ? "loss" : "none";
                } else if (property == "screen") {
                    const auto screen = trgBouncyGame->currentScreen();
                    variables[tokens[3]] = screen == bouncyballs::ScreenState::GAMEPLAY ? "gameplay" :
                        screen == bouncyballs::ScreenState::SHOP ? "shop" : screen == bouncyballs::ScreenState::EXTRA ? "extra" :
                        screen == bouncyballs::ScreenState::EDITOR ? "editor" : "menu";
                } else { reportError(i + 1, "unknown game query property", property); continue; }
                types[tokens[3]] = (property == "moving" || property == "lost" || property == "active" ||
                    property == "dragging" || property == "dark_mode" || property == "sound_muted" ||
                    property == "level_locked" || property == "editor_unlocked" || property == "developer_unlocked" ||
                    property == "full_trg") ? "toggle" : (property == "overlay" || property == "screen" ||
                    property == "equipped_skin" || property == "equipped_color" || property == "equipped_design" ||
                    property == "equipped_special" ? "word" : "num");
            } else if (action == "object" && tokens.size() == 5 && validName(tokens[4])) {
                double objectIndex = 0;
                if (!readNumber(2, objectIndex) || objectIndex < 0) { reportError(i + 1, "game object needs a non-negative index"); continue; }
                const auto& objects = trgBouncyGame->physicsWorld().objects;
                const auto index = static_cast<std::size_t>(objectIndex);
                const std::string property = lower(tokens[3]);
                if (index >= objects.size()) {
                    if (property == "type") variables[tokens[4]] = "none";
                    else variables[tokens[4]] = 0.0;
                    types[tokens[4]] = property == "type" ? "word" : "num";
                } else {
                    const auto& object = objects[index];
                    if (property == "x") variables[tokens[4]] = static_cast<double>(object.x);
                    else if (property == "y") variables[tokens[4]] = static_cast<double>(object.y);
                    else if (property == "w") variables[tokens[4]] = static_cast<double>(object.w);
                    else if (property == "h") variables[tokens[4]] = static_cast<double>(object.h);
                    else if (property == "angle") variables[tokens[4]] = static_cast<double>(object.angle);
                    else if (property == "circle") variables[tokens[4]] = object.isCircle;
                    else if (property == "type") {
                        variables[tokens[4]] = object.type == bouncyballs::GameObjectType::WALL ? "wall" :
                            object.type == bouncyballs::GameObjectType::LETHAL ? "lethal" :
                            object.type == bouncyballs::GameObjectType::FINISH ? "finish" :
                            object.type == bouncyballs::GameObjectType::FREEZE ? "freeze" : "slide";
                    } else { reportError(i + 1, "unknown game object property", property); continue; }
                    types[tokens[4]] = property == "circle" ? "toggle" : property == "type" ? "word" : "num";
                }
            } else if (action == "set" && tokens.size() == 4) {
                const std::string property = lower(tokens[2]);
                const std::string value = valueFor(tokens[3], variables);
                if (property == "coins") { double amount = 0; if (!parseNumber(value, amount)) reportError(i + 1, "coins must be numeric"); else trgBouncyGame->setCoins(static_cast<int>(amount)); }
                else if (property == "dark_mode") trgBouncyGame->setDarkMode(lower(value) == "true" || value == "1");
                else if (property == "sound_muted") trgBouncyGame->setSoundMuted(lower(value) == "true" || value == "1");
                else if (property == "level_locked") trgBouncyGame->setLevelLocked(lower(value) == "true" || value == "1");
                else if (property == "debug") trgBouncyGame->setDebugMode(lower(value) == "true" || value == "1");
                else if (property == "tester") trgBouncyGame->setTester(lower(value) == "true" || value == "1");
                else if (property == "developer_unlocked") trgBouncyGame->setDeveloperUnlocked(lower(value) == "true" || value == "1");
                else if (property == "full_trg") {
                    trgBouncyGame->setFullTrgEnabled(lower(value) == "true" || value == "1");
                    trgBouncyGame->requestFullTrgChanged();
                }
                else if (property == "editor_unlocked") trgBouncyGame->setEditorUnlocked(lower(value) == "true" || value == "1");
                else reportError(i + 1, "unknown game setting", property);
            } else if (action == "purchase" && tokens.size() == 6) {
                double cost = 0;
                const std::size_t resultIndex = tokens.size() - 1;
                const std::size_t costIndex = tokens.size() - 2;
                if (!readNumber(costIndex, cost) || cost < 0 || !validName(tokens[resultIndex])) reportError(i + 1, "use game purchase CATEGORY ID COST RESULT");
                else { variables[tokens[resultIndex]] = trgBouncyGame->purchaseItem(tokens[2], tokens[3], static_cast<int>(cost)); types[tokens[resultIndex]] = "toggle"; }
            } else if (action == "equip" && tokens.size() == 4) {
                const std::string category = lower(tokens[2]);
                if (category == "skin") trgBouncyGame->setEquippedSkin(tokens[3]);
                else if (category == "color") trgBouncyGame->setEquippedColor(tokens[3]);
                else if (category == "design") trgBouncyGame->setEquippedDesign(tokens[3]);
                else if (category == "special") trgBouncyGame->setEquippedSpecial(tokens[3]);
                else reportError(i + 1, "equipment category must be skin, color, design, or special", category);
            } else if (action == "editor" && tokens.size() >= 3) {
                const std::string operation = lower(tokens[2]);
                if (operation == "add" && tokens.size() == 4) {
                    const std::string kind = lower(tokens[3]);
                    if (kind == "wall") trgBouncyGame->addEditorPart(bouncyballs::GameObjectType::WALL);
                    else if (kind == "lethal" || kind == "hazard") trgBouncyGame->addEditorPart(bouncyballs::GameObjectType::LETHAL);
                    else if (kind == "finish") trgBouncyGame->addEditorPart(bouncyballs::GameObjectType::FINISH);
                    else if (kind == "freeze") trgBouncyGame->addEditorPart(bouncyballs::GameObjectType::FREEZE);
                    else if (kind == "slide") trgBouncyGame->addEditorPart(bouncyballs::GameObjectType::SLIDE);
                    else reportError(i + 1, "editor part type must be wall, lethal, finish, freeze, or slide", kind);
                } else if (operation == "delete" && tokens.size() == 3) trgBouncyGame->deleteSelectedEditorPart();
                else if (operation == "rotate" && tokens.size() == 4) {
                    double angle = 0;
                    if (!readNumber(3, angle)) reportError(i + 1, "editor rotate needs degrees");
                    else trgBouncyGame->rotateSelectedEditorPart(static_cast<float>(angle));
                } else if (operation == "resize" && tokens.size() == 5) {
                    double dw = 0, dh = 0;
                    if (!readNumber(3, dw) || !readNumber(4, dh)) reportError(i + 1, "editor resize needs width and height changes");
                    else trgBouncyGame->resizeSelectedEditorPart(static_cast<float>(dw), static_cast<float>(dh));
                } else if (operation == "playtest" && tokens.size() == 3) trgBouncyGame->playtestEditorLevel();
                else if (operation == "select" && tokens.size() == 4) {
                    double index = 0;
                    if (!readNumber(3, index) || index < -1) reportError(i + 1, "editor select needs an index of zero or more");
                    else trgBouncyGame->setSelectedEditorIndex(static_cast<int>(index));
                } else reportError(i + 1, "use game editor add TYPE|delete|rotate DEGREES|resize DW DH|playtest|select INDEX");
            } else {
                reportError(i + 1, "invalid game command or argument count", action);
            }
            continue;
        }
#endif

        
        
        if (cmd == "set") {
            if (tokens.size() < 3 || !validVariablePath(tokens[1])) {
                reportError(i + 1, "use set VARIABLE VALUE", "set");
                continue;
            }
            const std::size_t namePosition = positionAfterArgument(line, tokens[1]);
            if (namePosition == std::string::npos) { reportError(i + 1, "set needs a valid variable", tokens[1]); continue; }
            std::string raw = trim(line.substr(namePosition));
            if (!raw.empty() && raw.front() == '=') raw = trim(raw.substr(1));
            const std::string type = types.count(tokens[1]) ? types[tokens[1]] : "word";
            if (type == "num") {
                double number = 0;
                if (!evaluateExpression(raw, variables, number)) { reportError(i + 1, "set needs a valid number", raw); continue; }
                variables[tokens[1]] = number;
            } else {
                const auto parts = tokenize(raw);
                std::string value;
                for (std::size_t part = 0; part < parts.size(); ++part) {
                    if (part) value += ' ';
                    value += valueFor(parts[part], variables);
                }
                if (type == "toggle") {
                    const std::string flag = lower(value);
                    if (flag != "true" && flag != "false" && flag != "yes" && flag != "no" && value != "1" && value != "0") {
                        reportError(i + 1, "set toggle needs true or false", value); continue;
                    }
                    variables[tokens[1]] = flag == "true" || flag == "yes" || value == "1";
                } else if (type == "color") {
                    unsigned red = 0, green = 0, blue = 0;
                    if (!parseColor(value, red, green, blue)) { reportError(i + 1, "set color needs a valid color", value); continue; }
                    variables[tokens[1]] = value;
                } else if (type == "list") {
                    reportError(i + 1, "use push or remove to change list values", tokens[1]);
                    continue;
                } else variables[tokens[1]] = value;
            }
            if (!types.count(tokens[1])) types[tokens[1]] = type;
            continue;
        }
        if (cmd == "increase" || cmd == "decrease") {
            if ((tokens.size() != 2 && tokens.size() != 3) || !validVariablePath(tokens[1]) ||
                !variables.count(tokens[1]) || !types.count(tokens[1]) || types[tokens[1]] != "num") {
                reportError(i + 1, "use increase/decrease NUMBER_VARIABLE [AMOUNT]");
                continue;
            }
            const std::string amountText = tokens.size() == 3 ? tokens[2] : "1";
            double amount = 0, current = 0;
            if (!evaluateExpression(amountText, variables, amount) || !parseNumber(static_cast<std::string>(variables[tokens[1]]), current)) {
                reportError(i + 1, "increase/decrease needs numeric values", tokens[1]);
                continue;
            }
            variables[tokens[1]] = current + (cmd == "increase" ? amount : -amount);
            continue;
        }

        
        
        if (cmd == "record" || cmd == "struct" || cmd == "class") {
            std::size_t open = 0, close = 0;
            const bool headerShape = tokens.size() == 2 || (tokens.size() == 3 && tokens[2] == "(");
            if (!headerShape || !validName(tokens[1]) || !findBlock(program, i, end, open, close)) {
                reportError(i + 1, "use record NAME ( word field; num count = 0 )", tokens.size() > 1 ? tokens[1] : "");
                continue;
            }
            StructDefinition definition;
            for (std::size_t row = open + 1; row < close; ++row) {
                const auto field = tokenize(trim(program[row]));
                if (field.empty()) continue;
                const std::string fieldType = lower(field[0]);
                if (field.size() < 2 || !validName(field[1]) ||
                    (fieldType != "word" && fieldType != "num" && fieldType != "toggle" && fieldType != "color")) {
                    reportError(row + 1, "record fields use word, num, toggle, or color", field[0]);
                    continue;
                }
                std::string initial;
                const std::size_t equals = trim(program[row]).find('=');
                if (equals != std::string::npos) {
                    const auto initialTokens = tokenize(trim(program[row]).substr(equals + 1));
                    for (std::size_t part = 0; part < initialTokens.size(); ++part) {
                        if (part) initial += ' ';
                        initial += initialTokens[part];
                    }
                }
                definition[field[1]] = {fieldType, initial};
            }
            userStructs[tokens[1]] = std::move(definition);
            i = close;
            continue;
        }
        if (cmd == "new") {
            if (tokens.size() != 3 || !validName(tokens[2]) || !userStructs.count(tokens[1])) {
                reportError(i + 1, "use new TYPE instanceName (define TYPE with record first)");
                continue;
            }
            for (const auto& field : userStructs[tokens[1]]) {
                const std::string key = tokens[2] + "." + field.first;
                const std::string raw = field.second.initial;
                const std::string initial = raw.empty() ? defaults[field.second.type] : valueFor(raw, variables);
                if (field.second.type == "num") {
                    double number = 0;
                    if (!evaluateExpression(raw.empty() ? initial : raw, variables, number)) number = 0;
                    variables[key] = number;
                } else if (field.second.type == "toggle") variables[key] = lower(initial) == "true";
                else variables[key] = initial;
                types[key] = field.second.type;
            }
            continue;
        }

        
        if (tokens.size() == 3 && validName(tokens[0]) &&
            (std::any_of(uiBlocks.begin(), uiBlocks.end(), [&](const UiBlock& block) { return block.name == tokens[0]; }) ||
             std::any_of(uiSprites.begin(), uiSprites.end(), [&](const UiSprite& sprite) { return sprite.name == tokens[0]; }) ||
             std::any_of(uiButtons.begin(), uiButtons.end(), [&](const UiButton& button) { return button.name == tokens[0]; }) ||
             std::any_of(uiTexts.begin(), uiTexts.end(), [&](const UiText& text) { return text.name == tokens[0]; }))) {
            auto block = std::find_if(uiBlocks.begin(), uiBlocks.end(), [&](const UiBlock& item) { return item.name == tokens[0]; });
            auto sprite = std::find_if(uiSprites.begin(), uiSprites.end(), [&](const UiSprite& item) { return item.name == tokens[0]; });
            auto button = std::find_if(uiButtons.begin(), uiButtons.end(), [&](const UiButton& item) { return item.name == tokens[0]; });
            auto text = std::find_if(uiTexts.begin(), uiTexts.end(), [&](const UiText& item) { return item.name == tokens[0]; });
            const std::string property = lower(tokens[1]);
            const std::string raw = valueFor(tokens[2], variables);
            if (text != uiTexts.end() && (property == "text" || property == "value")) { text->value = raw; continue; }
            if (button != uiButtons.end() && (property == "label" || property == "text")) { button->label = raw; continue; }
            if (property == "visible" || property == "show") {
                const bool visible = lower(raw) == "true" || lower(raw) == "yes" || raw == "1";
                if (block != uiBlocks.end()) block->visible = visible;
                else if (sprite != uiSprites.end()) sprite->visible = visible;
                else if (button != uiButtons.end()) button->visible = visible;
                else text->visible = visible;
                continue;
            }
            if (text != uiTexts.end() && property == "color") {
                unsigned red = 0, green = 0, blue = 0;
                if (!parseColor(raw, red, green, blue)) reportError(i + 1, "invalid text color", raw);
                else text->color = raw;
                continue;
            }
            if (text != uiTexts.end() && (property == "font" || property == "align")) {
                const std::string value = lower(raw);
                if (property == "font" && value != "sans" && value != "serif" && value != "mono" && value != "monospace")
                    reportError(i + 1, "font must be sans, serif, or mono", raw);
                else if (property == "align" && value != "left" && value != "center" && value != "right")
                    reportError(i + 1, "text alignment must be left, center, or right", raw);
                else if (property == "font") text->font = value == "monospace" ? "mono" : value;
                else text->align = value;
                continue;
            }
            if (property == "edges" || property == "shape") {
                if (lower(raw) != "square" && lower(raw) != "round" && lower(raw) != "rounded") {
                    reportError(i + 1, "UI edges must be round or square", raw); continue;
                }
                if (block != uiBlocks.end()) block->cornerRadius = lower(raw) == "square" ? 0 : 10;
                else if (button != uiButtons.end()) button->cornerRadius = lower(raw) == "square" ? 0 : 7;
                continue;
            }
            if (property == "color" && (block != uiBlocks.end() || button != uiButtons.end())) {
                unsigned red = 0, green = 0, blue = 0;
                if (!parseColor(raw, red, green, blue)) reportError(i + 1, "invalid UI color", raw);
                else if (block != uiBlocks.end()) block->color = raw;
                else button->color = raw;
                continue;
            }
            if (property == "opacity" || property == "size") {
                double number = 0;
                if (!parseNumber(raw, number) || number < 0 || number > (property == "opacity" ? 1.0 : 100.0)) {
                    reportError(i + 1, "opacity must be 0..1 and text size must be 0..100", raw); continue;
                }
                if (block != uiBlocks.end() && property == "opacity") block->opacity = number;
                else if (sprite != uiSprites.end() && property == "opacity") sprite->opacity = number;
                else if (button != uiButtons.end() && property == "opacity") button->opacity = number;
                else if (text != uiTexts.end() && property == "opacity") text->opacity = number;
                else if (text != uiTexts.end()) text->size = number;
                else { reportError(i + 1, "opacity or size is not supported by this UI object", property); continue; }
                continue;
            }
            if (text != uiTexts.end() && property == "bold") { text->bold = lower(raw) == "true" || raw == "1"; continue; }
            if (button != uiButtons.end() && property == "font") {
                const std::string value = lower(raw);
                if (value == "sans" || value == "serif" || value == "mono" || value == "monospace") button->font = value == "monospace" ? "mono" : value;
                else reportError(i + 1, "button font must be sans, serif, or mono", raw);
                continue;
            }
            if (button != uiButtons.end() && property == "text_color") {
                unsigned red = 0, green = 0, blue = 0;
                if (!parseColor(raw, red, green, blue)) reportError(i + 1, "invalid button text color", raw);
                else button->textColor = raw;
                continue;
            }
            if (button != uiButtons.end() && property == "bold") { button->textBold = lower(raw) == "true" || raw == "1"; continue; }
            if (property == "x" || property == "y" || property == "w" || property == "h" || property == "radius") {
                double number = 0;
                const bool relative = raw.size() > 1 && (raw[0] == '+' || raw[0] == '-');
                if (!parseNumber(raw, number)) { reportError(i + 1, "UI position and size need a number", raw); continue; }
                if (block != uiBlocks.end()) {
                    if (property == "x") block->x = relative ? block->x + number : number;
                    else if (property == "y") block->y = relative ? block->y + number : number;
                    else if (property == "w") block->width = number;
                    else if (property == "h") block->height = number;
                    else block->cornerRadius = std::max(0.0, number);
                } else if (sprite != uiSprites.end()) {
                    if (property == "x") sprite->x = relative ? sprite->x + number : number;
                    else if (property == "y") sprite->y = relative ? sprite->y + number : number;
                    else if (property == "w") sprite->width = number;
                    else if (property == "h") sprite->height = number;
                } else if (button != uiButtons.end()) {
                    if (property == "x") button->x = relative ? button->x + number : number;
                    else if (property == "y") button->y = relative ? button->y + number : number;
                    else if (property == "w") button->width = number;
                    else if (property == "h") button->height = number;
                    else button->cornerRadius = std::max(0.0, number);
                } else if (text != uiTexts.end()) {
                    if (property == "x") text->x = relative ? text->x + number : number;
                    else if (property == "y") text->y = relative ? text->y + number : number;
                    else { reportError(i + 1, "text only supports x, y, size, and opacity", property); continue; }
                }
                refreshCollisionWatches(variables, types);
                continue;
            }
        }
        if (tokens.size() == 3 && std::any_of(uiButtons.begin(), uiButtons.end(),
                [&](const UiButton& button) { return button.name == tokens[0]; })) {
            auto button = std::find_if(uiButtons.begin(), uiButtons.end(),
                [&](const UiButton& item) { return item.name == tokens[0]; });
            const std::string property = lower(tokens[1]);
            const std::string raw = valueFor(tokens[2], variables);
            if (property == "color") {
                unsigned red = 0, green = 0, blue = 0;
                if (!parseColor(raw, red, green, blue)) reportError(i + 1, "invalid UI button color", raw);
                else button->color = raw;
            }
            else if (property == "edges" || property == "shape") {
                if (lower(raw) != "square" && lower(raw) != "round" && lower(raw) != "rounded") {
                    reportError(i + 1, "UI edges must be round or square", raw); continue;
                }
                button->cornerRadius = lower(raw) == "square" ? 0 : 7;
            }
            else {
                double number = 0;
                const bool relative = raw.size() > 1 && (raw[0] == '+' || raw[0] == '-');
                if (!parseNumber(raw, number)) { reportError(i + 1, "UI button property needs a number or color", raw); continue; }
                if (property == "x") button->x = relative ? button->x + number : number;
                else if (property == "y") button->y = relative ? button->y + number : number;
                else if (property == "w") button->width = number;
                else if (property == "h") button->height = number;
                else if (property == "radius") button->cornerRadius = std::max(0.0, number);
                else if (property == "opacity" && number >= 0 && number <= 1) button->opacity = number;
                else { reportError(i + 1, "unknown UI button property", property); continue; }
            }
            refreshCollisionWatches(variables, types);
            continue;
        }

        if ((cmd == "show" || cmd == "hide") && tokens.size() == 2) {
            const bool visible = cmd == "show";
            auto group = std::find_if(uiGroups.begin(), uiGroups.end(), [&](const UiGroup& item) { return item.name == tokens[1]; });
            if (group != uiGroups.end()) { group->visible = visible; continue; }
            bool found = false;
            for (UiBlock& item : uiBlocks) if (item.name == tokens[1]) { item.visible = visible; found = true; }
            for (UiSprite& item : uiSprites) if (item.name == tokens[1]) { item.visible = visible; found = true; }
            for (UiButton& item : uiButtons) if (item.name == tokens[1]) { item.visible = visible; found = true; }
            for (UiText& item : uiTexts) if (item.name == tokens[1]) { item.visible = visible; found = true; }
            if (!found) reportError(i + 1, "unknown UI group or object", tokens[1]);
            continue;
        }

        
        auto ufIt = userFunctions.find(cmd);
        if (ufIt != userFunctions.end()) {
            const auto& uf = ufIt->second;
            Vars savedVars;
            for (std::size_t p = 0; p < uf.params.size() && p + 1 < tokens.size(); ++p) {
                const auto existing = variables.find(uf.params[p]);
                if (existing != variables.end()) savedVars[uf.params[p]] = existing->second;
                variables[uf.params[p]] = valueFor(tokens[p + 1], variables);
            }
            Flow inner;
            executeBlock(uf.body, 0, uf.body.size(), variables, types, defaults, inner, manualMode, uiMode);
            for (const std::string& param : uf.params) {
                const auto saved = savedVars.find(param);
                if (saved == savedVars.end()) variables.erase(param);
                else variables[param] = saved->second;
            }
            if (inner.stopped) { flow.stopped = true; return; }
            continue;
        }
        if (cmd == "define") {
            std::size_t open = 0, close = 0;
            if (!findBlock(program, i, end, open, close)) {
                reportError(i + 1, "function definition needs a code block");
                continue;
            }
            i = close;
            continue;
        }

        
        if (cmd == "say") {
            std::string message;
            for (std::size_t a = 1; a < tokens.size(); ++a) {
                if (a > 1) message += ' ';
                message += valueFor(tokens[a], variables);
            }
            appendOutput(message + "\r\n");
            continue;
        }
        if (cmd == "wait") {
            if (tokens.size() != 2) { reportError(i + 1, "use wait 10s or wait 100ms"); continue; }
            std::string duration = lower(valueFor(tokens[1], variables));
            std::chrono::milliseconds delay(0);
            try {
                if (duration.size() > 2 && duration.substr(duration.size() - 2) == "ms") {
                    double value = 0;
                    if (!parseNumber(duration.substr(0, duration.size() - 2), value) || value < 0) throw std::invalid_argument("duration");
                    delay = std::chrono::milliseconds(static_cast<long long>(value));
                } else {
                    if (!duration.empty() && duration.back() == 's') duration.pop_back();
                    double value = 0;
                    if (!parseNumber(duration, value) || value < 0) throw std::invalid_argument("duration");
                    delay = std::chrono::milliseconds(static_cast<long long>(value * 1000));
                }
            } catch (...) { reportError(i + 1, "wait duration must look like 100ms or 10s", tokens[1]); continue; }
#ifdef __ANDROID__
            const auto until = std::chrono::steady_clock::now() + delay;
            while (!androidStopRequested.load() && std::chrono::steady_clock::now() < until)
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
#else
            std::this_thread::sleep_for(delay);
#endif
            continue;
        }
        if (cmd == "play") {
            if (tokens.size() < 2) { reportError(i + 1, "use play FILE"); continue; }
            std::string fileName = valueFor(tokens[1], variables);
            bool pathValid = false;
            const std::filesystem::path mediaPath = projectFilePath(fileName, pathValid);
            if (!pathValid || !std::filesystem::is_regular_file(mediaPath)) {
                reportError(i + 1, "media file not found inside project folder", fileName); continue;
            }
            std::string ext = lower(mediaPath.extension().u8string());
            if (ext == ".m4a" || ext == ".mp3" || ext == ".wav" || ext == ".ogg" || ext == ".aac") {
#ifdef _WIN32
                std::wstring wpath = mediaPath.wstring();
                if (!PlaySoundW(wpath.c_str(), nullptr, SND_FILENAME | SND_ASYNC)) {
                    reportError(i + 1, "could not play sound", fileName);
                }
#else
                reportError(i + 1, "play sound is Windows-only", fileName);
#endif
            } else if (ext == ".mp4" || ext == ".webm" || ext == ".avi" || ext == ".mov") {
#ifdef _WIN32
                ShellExecuteW(mainWindow, L"open", mediaPath.wstring().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
#else
                reportError(i + 1, "play video is Windows-only", fileName);
#endif
            } else {
                reportError(i + 1, "unknown media type", ext);
            }
            continue;
        }
        if (cmd == "scroll") {
            if (tokens.size() != 3) { reportError(i + 1, "use scroll X Y"); continue; }
            double vx = 0, vy = 0;
            if (!evaluateExpression(valueFor(tokens[1], variables), variables, vx) ||
                !evaluateExpression(valueFor(tokens[2], variables), variables, vy)) {
                reportError(i + 1, "scroll needs numeric X Y"); continue;
            }
            viewportX = vx;
            viewportY = vy;
            continue;
        }
        if (cmd == "http_get") {
            if (!networkEnabled) { reportError(i + 1, "http_get needs add (network)"); continue; }
            if (tokens.size() != 3 || !validName(tokens[2])) { reportError(i + 1, "use http_get URL RESULT"); continue; }
            std::string url = valueFor(tokens[1], variables);
            std::string error;
            std::string response = httpGet(url, error);
            if (!error.empty()) { reportError(i + 1, error, url); continue; }
            variables[tokens[2]] = response;
            types[tokens[2]] = "word";
            continue;
        }
        if (cmd == "http_post") {
            if (!networkEnabled) { reportError(i + 1, "http_post needs add (network)"); continue; }
            if (tokens.size() != 4 || !validName(tokens[3])) { reportError(i + 1, "use http_post URL BODY RESULT"); continue; }
            std::string url = valueFor(tokens[1], variables);
            std::string body = valueFor(tokens[2], variables);
            std::string error;
            std::string response = httpPost(url, body, error);
            if (!error.empty()) { reportError(i + 1, error, url); continue; }
            variables[tokens[3]] = response;
            types[tokens[3]] = "word";
            continue;
        }
        if (cmd == "particle") {
            // particle NAME X Y COLOR SPEED LIFE SIZE
            if (tokens.size() < 7) { reportError(i + 1, "use particle NAME X Y COLOR SPEED LIFE SIZE"); continue; }
            Particle p;
            p.name = tokens[1];
            if (!evaluateExpression(valueFor(tokens[2], variables), variables, p.x)) p.x = 0;
            if (!evaluateExpression(valueFor(tokens[3], variables), variables, p.y)) p.y = 0;
            p.color = valueFor(tokens[4], variables);
            if (!evaluateExpression(valueFor(tokens[5], variables), variables, p.vx)) p.vx = 0;
            p.vy = p.vx;
            if (!evaluateExpression(valueFor(tokens[6], variables), variables, p.maxLife)) p.maxLife = 1;
            p.life = p.maxLife;
            p.size = 4;
            if (tokens.size() >= 8) {
                if (!evaluateExpression(valueFor(tokens[7], variables), variables, p.size)) p.size = 4;
            }
            particles.push_back(p);
            continue;
        }
        if (cmd == "studio") {
#ifdef __ANDROID__
            if (tokens.size() != 2) {
                reportError(i + 1, "use studio ACTION");
            } else {
                trgStudioActionAndroid(tokens[1]);
            }
#else
            reportError(i + 1, "studio actions are only available in TRG Studio");
#endif
            continue;
        }
        if (cmd == "ask") {
            if (tokens.size() < 3 || !validName(tokens.back())) {
                reportError(i + 1, "ask needs a prompt and valid variable name");
                continue;
            }
            const std::size_t targetIndex = tokens.size() - 1;
            std::string prompt;
            for (std::size_t a = 1; a < targetIndex; ++a) {
                if (a > 1) prompt += ' ';
                prompt += valueFor(tokens[a], variables);
            }
            std::string answer;
            if (!askForInput(prompt, answer)) { reportError(i + 1, "input was cancelled"); continue; }
            const std::string target = tokens[targetIndex];
            if (!types.count(target)) types[target] = "word";
            if (types[target] == "num") {
                double numeric = 0;
                if (!parseNumber(answer, numeric)) { reportError(i + 1, "input must be a number", answer); continue; }
                variables[target] = numeric;
            } else if (types[target] == "toggle") {
                const std::string toggleValue = lower(answer);
                if (toggleValue != "true" && toggleValue != "false") { reportError(i + 1, "input must be true or false", answer); continue; }
                variables[target] = toggleValue == "true";
            } else variables[target] = answer;
            continue;
        }

        
        
        
        if (cmd == "add" && tokens.size() >= 2) {
            const std::string second = lower(tokens[1]);

            
            if (second == "global" || second == "(global)") {
                std::size_t open = 0, close = 0;
                if (!findBlock(program, i, end, open, close)) {
                    reportError(i + 1, "add global needs a ( ... ) block");
                    continue;
                }
                Flow inner;
                executeBlock(program, open + 1, close, variables, types, defaults, inner, manualMode, uiMode);
                if (inner.stopped) { flow.stopped = true; return; }
                i = close;
                continue;
            }

            
            if (second == "connect" || second == "(connect)") {
                std::string target;
                if (second == "(connect)") {
                    if (tokens.size() >= 3) target = tokens[2];
                } else {
                    target = parenContent(line);
                }
                if (target.empty() && tokens.size() >= 3) target = tokens[2];
                if (target.size() >= 2 && target.front() == '(' && target.back() == ')')
                    target = target.substr(1, target.size() - 2);
                if (target.empty()) { reportError(i + 1, "use add connect (file.trg)"); continue; }
                bool pathValid = false;
                const std::filesystem::path connectedPath = projectExistingFile(target, pathValid);
                if (!pathValid) { reportError(i + 1, "connected file must stay inside the TRG project folder", target); continue; }
                if (executedFileKeys.count(lower(canonicalProjectPath(connectedPath).u8string()))) continue;
                static thread_local unsigned connectDepth = 0;
                if (connectDepth >= 16) { reportError(i + 1, "connect depth exceeded", target); continue; }
                std::ifstream connectedFile(connectedPath, std::ios::binary);
                if (!connectedFile) { reportError(i + 1, "could not open connected file", target); continue; }
                const std::vector<std::string> connectedProgram = mergePrograms(readProgram(connectedFile), connectedPath, false, true, currentSourcePath);
                LibraryState connectedLibraries;
                scanLibraries(connectedProgram, connectedLibraries);
                filesEnabled = filesEnabled || connectedLibraries.files;
                jsonEnabled = jsonEnabled || connectedLibraries.json;
                webEnabled = webEnabled || connectedLibraries.web;
                collisionEnabled = collisionEnabled || connectedLibraries.collision;
                if (connectedLibraries.controls) initializeControls(variables, types);
                registerFunctions(connectedProgram, false);
                ++connectDepth;
                Flow inner;
                executeBlock(connectedProgram, 0, connectedProgram.size(), variables, types, defaults, inner,
                             manualMode || connectedLibraries.manual, uiMode || connectedLibraries.ui);
                --connectDepth;
                if (inner.stopped) { flow.stopped = true; return; }
                continue;
            }

            
            if (tokens.size() == 2 && tokens[1].size() >= 6 &&
                tokens[1].front() == '(' && tokens[1].back() == ')') {
                const std::string moduleName = tokens[1].substr(1, tokens[1].size() - 2);
                const std::string moduleLower = lower(moduleName);
                if (moduleLower.size() >= 4 && moduleLower.compare(moduleLower.size() - 4, 4, ".trg") == 0) {
                    static thread_local unsigned includeDepth = 0;
                    if (includeDepth >= 16) { reportError(i + 1, "module include depth exceeded", tokens[1]); continue; }
                    bool pathValid = false;
                    const std::filesystem::path modulePath = projectExistingFile(moduleName, pathValid);
                    if (!pathValid) { reportError(i + 1, "module must be inside the TRG project folder", moduleName); continue; }
                    std::ifstream module(modulePath, std::ios::binary);
                    if (!module) { reportError(i + 1, "could not open TRG module", moduleName); continue; }
                    const std::vector<std::string> moduleProgram = readProgram(module);
                    LibraryState moduleLibraries;
                    scanLibraries(moduleProgram, moduleLibraries);
                    filesEnabled = filesEnabled || moduleLibraries.files;
                    jsonEnabled = jsonEnabled || moduleLibraries.json;
                    webEnabled = webEnabled || moduleLibraries.web;
                    collisionEnabled = collisionEnabled || moduleLibraries.collision;
                    if (moduleLibraries.controls) initializeControls(variables, types);
                    registerFunctions(moduleProgram, false);
                    ++includeDepth;
                    Flow moduleFlow;
                    executeBlock(moduleProgram, 0, moduleProgram.size(), variables, types, defaults, moduleFlow,
                                 manualMode || moduleLibraries.manual, uiMode || moduleLibraries.ui);
                    --includeDepth;
                    if (moduleFlow.stopped) { flow.stopped = true; return; }
                    continue;
                }
            }

            
            if (tokens.size() == 2 && tokens[1].size() >= 3 &&
                tokens[1].front() == '(' && tokens[1].back() == ')') {
                const std::string library = lower(tokens[1]);
                if (library == "(files)") { filesEnabled = true; appendOutput("TRG files library enabled.\r\n"); continue; }
                if (library == "(json)") { jsonEnabled = true; appendOutput("TRG JSON library enabled.\r\n"); continue; }
                if (library == "(web)") { webEnabled = true; appendOutput("TRG WebAssembly commands enabled.\r\n"); continue; }
                if (library == "(collision)") { collisionEnabled = true; appendOutput("TRG collision helpers enabled.\r\n"); continue; }
                if (library == "(manual)") { manualMode = true; continue; }
                if (library == "(ui)" || library == "(animation)") { uiMode = true; continue; }
                if (library == "(controls)") { initializeControls(variables, types); continue; }
                if (library == "(network)") { networkEnabled = true; appendOutput("TRG network library enabled.\\r\\n"); continue; }
                if (library == "(all)") {
                    filesEnabled = jsonEnabled = webEnabled = collisionEnabled = networkEnabled = true;
                    initializeControls(variables, types);
                    manualMode = true;
                    uiMode = true;
                    appendOutput("All built-in TRG libraries enabled.\r\n");
                    continue;
                }
                reportError(i + 1, "unknown library", tokens[1]);
                continue;
            }

            
            if (!line.empty() && (line.back() == '(' || line.back() == '{')) {
                if (line.back() == '(') {
                    std::size_t open = 0, close = 0;
                    if (findBlock(program, i, end, open, close)) i = close;
                }
                reportError(i + 1, "libraries apply to the whole file; use add (json) without a block");
                continue;
            }
        }

        
        if (cmd == "run") {
            std::string target = parenContent(line);
            if (target.empty() && tokens.size() >= 2) {
                target = tokens[1];
                if (target.size() >= 2 && target.front() == '(' && target.back() == ')')
                    target = target.substr(1, target.size() - 2);
            }
            if (target.empty()) { reportError(i + 1, "use run (file.trg)"); continue; }
            bool pathValid = false;
            const std::filesystem::path runPath = projectExistingFile(target, pathValid);
            if (!pathValid) { reportError(i + 1, "run target must stay inside the TRG project folder", target); continue; }
            const std::string runKey = lower(canonicalProjectPath(runPath).u8string());
            
            
            
            if (activeRunFiles.count(runKey)) continue;
            static thread_local unsigned runDepth = 0;
            if (runDepth >= 16) { reportError(i + 1, "run depth exceeded", target); continue; }
            std::ifstream runFile(runPath, std::ios::binary);
            if (!runFile) { reportError(i + 1, "could not open file to run", target); continue; }
            const std::vector<std::string> source = readProgram(runFile);
            std::vector<std::string> runProgram;
            if (executedFileKeys.count(runKey)) {
                
                
                std::vector<std::string> kept, globals, connects;
                std::unordered_set<std::string> seen;
                stripProjectDirectives(source, kept, globals, connects, true, &seen);
                runProgram = std::move(kept);
            } else {
                runProgram = mergePrograms(source, runPath, false, true, currentSourcePath);
            }
            LibraryState runLibraries;
            scanLibraries(runProgram, runLibraries);
            filesEnabled = filesEnabled || runLibraries.files;
            jsonEnabled = jsonEnabled || runLibraries.json;
            webEnabled = webEnabled || runLibraries.web;
            collisionEnabled = collisionEnabled || runLibraries.collision;
            if (runLibraries.controls) initializeControls(variables, types);
            registerFunctions(runProgram, false);
            ++runDepth;
            activeRunFiles.insert(runKey);
            Flow inner;
            executeBlock(runProgram, 0, runProgram.size(), variables, types, defaults, inner,
                         manualMode || runLibraries.manual, uiMode || runLibraries.ui);
            activeRunFiles.erase(runKey);
            --runDepth;
            if (inner.stopped) { flow.stopped = true; return; }
            continue;
        }

        
        if (cmd == "json") {
            if (!jsonEnabled) { reportError(i + 1, "JSON commands need add (json)"); continue; }
            if (tokens.size() >= 2 && lower(tokens[1]) == "read") {
                std::string target = parenContent(line);
                if (target.empty() && tokens.size() >= 3) {
                    target = tokens[2];
                    if (target.size() >= 2 && target.front() == '(' && target.back() == ')')
                        target = target.substr(1, target.size() - 2);
                }
                if (target.empty()) { reportError(i + 1, "use json read (file.json)"); continue; }
                bool pathValid = false;
                const std::filesystem::path jsonPath = projectExistingFile(target, pathValid);
                if (!pathValid) { reportError(i + 1, "JSON file must stay inside the TRG project folder", target); continue; }
                std::string jsonError;
                if (!readJsonIntoVariables(jsonPath, variables, types, jsonError)) {
                    reportError(i + 1, jsonError, target);
                    continue;
                }
                continue;
            }
            if (tokens.size() < 4) { reportError(i + 1, "use json GROUP clean|compressed RESULT"); continue; }
            const std::string groupName = tokens[1];
            const std::string format = lower(tokens[2]);
            const std::string resultName = tokens.back();
            if (format != "clean" && format != "compressed") {
                reportError(i + 1, "json format must be clean or compressed", tokens[2]);
                continue;
            }
            if (!validName(resultName)) {
                reportError(i + 1, "json needs a valid result variable", resultName);
                continue;
            }
            std::vector<std::pair<std::string, VariableValue>> entries;
            std::unordered_set<std::string> included;
            auto include = [&](const std::string& name) {
                if (!included.insert(name).second) return;
                const auto it = variables.find(name);
                if (it != variables.end()) entries.push_back({it->first, it->second});
            };
            for (const auto& item : variables) {
                const auto membership = variableGroups.find(item.first);
                if (membership == variableGroups.end()) continue;
                if (std::find(membership->second.begin(), membership->second.end(), groupName) == membership->second.end()) continue;
                include(item.first);
            }
            
            
            for (std::size_t level = 0; level < groupStack.size() && level < groupSnapshots.size(); ++level) {
                if (groupStack[level] != groupName) continue;
                for (const auto& item : variables)
                    if (!groupSnapshots[level].count(item.first)) include(item.first);
            }
            variables[resultName] = serializeJsonObject(entries, format == "clean");
            types[resultName] = "word";
            continue;
        }

        
        if (cmd == "create") {
            if (!filesEnabled && !jsonEnabled) { reportError(i + 1, "create needs add (files) or add (json)"); continue; }
            if (tokens.size() < 3) { reportError(i + 1, "use create FILE (LOCATION) RESULT"); continue; }
            const std::string fileName = tokens[1];
            const std::string location = parenContent(line);
            const std::string resultName = tokens.back();
            if (!validName(resultName)) { reportError(i + 1, "create needs a valid result variable", resultName); continue; }
            const auto result = variables.find(resultName);
            if (result == variables.end()) { reportError(i + 1, "create needs an existing variable holding JSON", resultName); continue; }
            const std::string content = static_cast<std::string>(result->second);
            const std::string relative = location.empty() ? fileName : location + "/" + fileName;
            bool pathValid = false;
            const std::filesystem::path outputPath = projectFilePath(relative, pathValid);
            if (!pathValid) { reportError(i + 1, "file must stay inside the TRG project folder", relative); continue; }
            std::error_code directoryError;
            if (!outputPath.parent_path().empty())
                std::filesystem::create_directories(outputPath.parent_path(), directoryError);
            std::ofstream output(outputPath, std::ios::binary | std::ios::trunc);
            if (!output) { reportError(i + 1, "could not write JSON file", relative); continue; }
            output << content;
            continue;
        }

        if (cmd == "random") {
            if (tokens.size() != 4 || !validName(tokens[3])) {
                reportError(i + 1, "use random MIN MAX RESULT");
                continue;
            }
            double minimum = 0, maximum = 0;
            if (!evaluateExpression(tokens[1], variables, minimum) ||
                !evaluateExpression(tokens[2], variables, maximum) || minimum > maximum) {
                reportError(i + 1, "random needs numeric bounds with MIN no greater than MAX");
                continue;
            }
            if (std::floor(minimum) == minimum && std::floor(maximum) == maximum &&
                minimum >= static_cast<double>(std::numeric_limits<int>::min()) &&
                maximum <= static_cast<double>(std::numeric_limits<int>::max())) {
                std::uniform_int_distribution<int> distribution(static_cast<int>(minimum), static_cast<int>(maximum));
                variables[tokens[3]] = static_cast<double>(distribution(trgRandomEngine));
            } else {
                std::uniform_real_distribution<double> distribution(minimum, maximum);
                variables[tokens[3]] = distribution(trgRandomEngine);
            }
            types[tokens[3]] = "num";
            continue;
        }

        if (cmd == "collides" || cmd == "touching") {
            if (!collisionEnabled) { reportError(i + 1, "collision helpers need add (collision)"); continue; }
            if (tokens.size() != 4 || !validName(tokens[3])) {
                reportError(i + 1, "use collides OBJECT_A OBJECT_B RESULT");
                continue;
            }
            double ax = 0, ay = 0, aw = 0, ah = 0, bx = 0, by = 0, bw = 0, bh = 0;
            if (!uiEntityBounds(tokens[1], ax, ay, aw, ah) || !uiEntityBounds(tokens[2], bx, by, bw, bh)) {
                reportError(i + 1, "collision checks need two named UI blocks, buttons, or sprites");
                continue;
            }
            const bool overlap = ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by;
            variables[tokens[3]] = overlap;
            types[tokens[3]] = "toggle";
            continue;
        }
        if (cmd == "watch_collision") {
            if (!collisionEnabled) { reportError(i + 1, "collision helpers need add (collision)"); continue; }
            if (tokens.size() != 4 || !validName(tokens[3])) {
                reportError(i + 1, "use watch_collision OBJECT_A OBJECT_B RESULT");
                continue;
            }
            double x = 0, y = 0, w = 0, h = 0;
            if (!uiEntityBounds(tokens[1], x, y, w, h) || !uiEntityBounds(tokens[2], x, y, w, h)) {
                reportError(i + 1, "watch_collision needs two named UI blocks, buttons, or sprites");
                continue;
            }
            collisionWatches.push_back({tokens[1], tokens[2], tokens[3]});
            types[tokens[3]] = "toggle";
            refreshCollisionWatches(variables, types);
            continue;
        }
        if (cmd == "move_ui") {
            if (!collisionEnabled) { reportError(i + 1, "game movement helpers need add (collision)"); continue; }
            if (tokens.size() != 6 || lower(tokens[2]) != "x" || lower(tokens[4]) != "y") {
                reportError(i + 1, "use move_ui OBJECT x DX y DY");
                continue;
            }
            double dx = 0, dy = 0;
            if (!parseNumber(valueFor(tokens[3], variables), dx) || !parseNumber(valueFor(tokens[5], variables), dy)) {
                reportError(i + 1, "move_ui needs numeric x and y amounts");
                continue;
            }
            auto block = std::find_if(uiBlocks.begin(), uiBlocks.end(), [&](const UiBlock& item) { return item.name == tokens[1]; });
            auto sprite = std::find_if(uiSprites.begin(), uiSprites.end(), [&](const UiSprite& item) { return item.name == tokens[1]; });
            auto button = std::find_if(uiButtons.begin(), uiButtons.end(), [&](const UiButton& item) { return item.name == tokens[1]; });
            if (block != uiBlocks.end()) { block->x += dx; block->y += dy; }
            else if (sprite != uiSprites.end()) { sprite->x += dx; sprite->y += dy; }
            else if (button != uiButtons.end()) { button->x += dx; button->y += dy; }
            else { reportError(i + 1, "unknown game object", tokens[1]); continue; }
            refreshCollisionWatches(variables, types);
            continue;
        }

        if (cmd == "list") {
            if (tokens.size() < 2 || !validName(tokens[1])) { reportError(i + 1, "use list NAME [VALUE, VALUE]", "list"); continue; }
            std::vector<std::string> items;
            const std::size_t namePosition = positionAfterArgument(line, tokens[1]);
            if (namePosition == std::string::npos) { reportError(i + 1, "list needs a valid name", tokens[1]); continue; }
            std::string initial = trim(line.substr(namePosition));
            if (!initial.empty() && initial.front() == '=') {
                initial = trim(initial.substr(1));
                items = tokenize(initial);
            }
            variables[tokens[1]] = std::move(items);
            types[tokens[1]] = "list";
            continue;
        }

        if (cmd == "push" || cmd == "get" || cmd == "remove") {
            if (tokens.size() != (cmd == "get" ? 4u : 3u) || !variables.count(tokens[1]) || types[tokens[1]] != "list") {
                reportError(i + 1, cmd == "push" ? "use push LIST VALUE" : (cmd == "get" ? "use get LIST INDEX VARIABLE" : "use remove LIST INDEX"));
                continue;
            }
            auto* items = std::get_if<std::vector<std::string>>(&variables[tokens[1]].value);
            if (!items) { reportError(i + 1, "variable is not a list", tokens[1]); continue; }
            if (cmd == "push") items->push_back(valueFor(tokens[2], variables));
            else {
                double parsedIndex = 0;
                if (!parseNumber(valueFor(tokens[2], variables), parsedIndex) || parsedIndex < 0 ||
                    parsedIndex >= static_cast<double>(items->size())) { reportError(i + 1, "list index is out of range", tokens[2]); continue; }
                const std::size_t index = static_cast<std::size_t>(parsedIndex);
                if (cmd == "get") {
                    if (!validName(tokens[3])) { reportError(i + 1, "get needs a valid destination variable", tokens[3]); continue; }
                    variables[tokens[3]] = (*items)[index];
                    if (!types.count(tokens[3])) types[tokens[3]] = "word";
                } else items->erase(items->begin() + static_cast<std::ptrdiff_t>(index));
            }
            continue;
        }

        if (cmd == "upper" || cmd == "lower" || cmd == "length" || cmd == "contains" || cmd == "replace") {
            const std::size_t expected = cmd == "replace" ? 5 : (cmd == "contains" ? 4 : 3);
            if (tokens.size() != expected || !validName(tokens.back())) {
                reportError(i + 1, cmd == "replace" ? "use replace VALUE SEARCH REPLACEMENT RESULT" :
                    (cmd == "contains" ? "use contains VALUE SEARCH RESULT" : "use " + cmd + " VALUE RESULT"));
                continue;
            }
            const std::string source = valueFor(tokens[1], variables);
            std::string result;
            if (cmd == "upper") { result = source; std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return static_cast<char>(std::toupper(c)); }); }
            else if (cmd == "lower") result = lower(source);
            else if (cmd == "length") {
                const auto variable = variables.find(tokens[1]);
                const auto* listValue = variable == variables.end() ? nullptr : std::get_if<std::vector<std::string>>(&variable->second.value);
                result = std::to_string(listValue ? listValue->size() : source.size());
            } else if (cmd == "contains") result = source.find(valueFor(tokens[2], variables)) == std::string::npos ? "false" : "true";
            else {
                const std::string search = valueFor(tokens[2], variables);
                const std::string replacement = valueFor(tokens[3], variables);
                if (!search.empty()) {
                    result = source;
                    std::size_t position = 0;
                    while ((position = result.find(search, position)) != std::string::npos) {
                        result.replace(position, search.size(), replacement);
                        position += replacement.size();
                    }
                } else result = source;
            }
            if (cmd == "contains") { variables[tokens.back()] = result == "true"; types[tokens.back()] = "toggle"; }
            else if (cmd == "length" && !(variables.count(tokens[1]) && types[tokens[1]] == "list")) {
                double numeric = 0; parseNumber(result, numeric); variables[tokens.back()] = numeric; types[tokens.back()] = "num";
            } else if (cmd == "length") { double numeric = 0; parseNumber(result, numeric); variables[tokens.back()] = numeric; types[tokens.back()] = "num"; }
            else { variables[tokens.back()] = result; if (!types.count(tokens.back())) types[tokens.back()] = "word"; }
            continue;
        }

        if (cmd == "read_file" || cmd == "write_file" || cmd == "append_file" || cmd == "delete_file" || cmd == "file_exists") {
            const std::size_t minimum = cmd == "write_file" || cmd == "append_file" ? 3 :
                (cmd == "read_file" || cmd == "file_exists" ? 3 : 2);
            if (!filesEnabled) { reportError(i + 1, "enable file commands with add (files)"); continue; }
            if (tokens.size() < minimum) { reportError(i + 1, "missing file command arguments", cmd); continue; }
            bool pathValid = false;
            const std::filesystem::path filePath = projectFilePath(valueFor(tokens[1], variables), pathValid);
            if (!pathValid) { reportError(i + 1, "file path must stay inside the TRG project folder", tokens[1]); continue; }
            try {
                if (cmd == "read_file") {
                    if (!validName(tokens[2])) { reportError(i + 1, "read_file needs a destination variable", tokens[2]); continue; }
                    std::ifstream input(filePath, std::ios::binary);
                    if (!input) { reportError(i + 1, "could not read file", tokens[1]); continue; }
                    std::ostringstream content; content << input.rdbuf();
                    variables[tokens[2]] = content.str();
                    types[tokens[2]] = "word";
                } else if (cmd == "file_exists") {
                    if (!validName(tokens[2])) { reportError(i + 1, "file_exists needs a destination variable", tokens[2]); continue; }
                    variables[tokens[2]] = std::filesystem::exists(filePath);
                    types[tokens[2]] = "toggle";
                } else if (cmd == "delete_file") {
                    if (!std::filesystem::remove(filePath)) reportError(i + 1, "file does not exist", tokens[1]);
                } else {
                    std::ofstream output(filePath, std::ios::binary | (cmd == "append_file" ? std::ios::app : std::ios::trunc));
                    if (!output) { reportError(i + 1, "could not write file", tokens[1]); continue; }
                    for (std::size_t n = 2; n < tokens.size(); ++n) { if (n > 2) output << ' '; output << valueFor(tokens[n], variables); }
                    output << '\n';
                    if (!output) reportError(i + 1, "failed while writing file", tokens[1]);
                }
            } catch (const std::filesystem::filesystem_error& error) { reportError(i + 1, error.what(), tokens[1]); }
            continue;
        }

        if (cmd == "play_audio" || cmd == "show_image" || cmd == "play_video") {
            if (tokens.size() != 2) { reportError(i + 1, "use " + cmd + " PROJECT_FILE"); continue; }
            bool pathValid = false;
            const std::filesystem::path mediaPath = projectFilePath(valueFor(tokens[1], variables), pathValid);
            if (!pathValid || !std::filesystem::is_regular_file(mediaPath)) {
                reportError(i + 1, "media file must exist inside the TRG project folder", tokens[1]);
                continue;
            }
#ifdef __ANDROID__
            trgStudioActionAndroid("media:" + cmd + ":" + mediaPath.u8string());
#elif defined(_WIN32)
            const std::wstring widePath = mediaPath.wstring();
            const HINSTANCE result = ShellExecuteW(mainWindow, L"open", widePath.c_str(), nullptr,
                                                    projectRoot.wstring().c_str(), SW_SHOWNORMAL);
            if (reinterpret_cast<INT_PTR>(result) <= 32) reportError(i + 1, "could not open media file", tokens[1]);
#else
            reportError(i + 1, "media playback is not supported on this platform", tokens[1]);
#endif
            continue;
        }

        if (cmd == "web_page") {
            if (!webEnabled) { reportError(i + 1, "enable TRG Web with add (web)"); continue; }
            if (tokens.size() < 2) { reportError(i + 1, "use web_page TITLE"); continue; }
            std::string title;
            for (std::size_t n = 1; n < tokens.size(); ++n) { if (n > 1) title += ' '; title += valueFor(tokens[n], variables); }
            const std::string safeTitle = htmlEscape(title);
            webPageHtml = "<!doctype html><html><head><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\"><title>" + safeTitle +
                "</title><style>body{margin:0;padding:24px;font:18px system-ui;background:#20202a;color:#f3f0fa}button{padding:12px 20px;border:0;border-radius:8px;background:#7043be;color:white;font-size:16px}</style></head><body><h1>" + safeTitle + "</h1>";
            continue;
        }

        if (cmd == "web_text" || cmd == "web_heading") {
            if (!webEnabled || webPageHtml.empty()) { reportError(i + 1, "start a TRG Web page with add (web) and web_page TITLE"); continue; }
            if (tokens.size() < 2) { reportError(i + 1, "web text needs a value"); continue; }
            std::string value;
            for (std::size_t n = 1; n < tokens.size(); ++n) { if (n > 1) value += ' '; value += valueFor(tokens[n], variables); }
            const std::string escaped = htmlEscape(value);
            webPageHtml += cmd == "web_heading" ? "<h2>" + escaped + "</h2>" : "<p>" + escaped + "</p>";
            continue;
        }

        if (cmd == "web_button") {
            if (!webEnabled || webPageHtml.empty() || tokens.size() < 2) { reportError(i + 1, "use web_button LABEL [MESSAGE] after web_page"); continue; }
            const std::string label = htmlEscape(valueFor(tokens[1], variables));
            const std::string message = htmlEscape(tokens.size() > 2 ? valueFor(tokens[2], variables) : valueFor(tokens[1], variables));
            webPageHtml += "<button data-message=\"" + message + "\" onclick=\"alert(this.dataset.message)\">" + label + "</button> ";
            continue;
        }

        if (cmd == "web_color") {
            if (!webEnabled || webPageHtml.empty() || tokens.size() != 2) { reportError(i + 1, "use web_color COLOR after web_page"); continue; }
            unsigned red = 0, green = 0, blue = 0;
            const std::string color = valueFor(tokens[1], variables);
            if (!parseColor(color, red, green, blue)) { reportError(i + 1, "invalid TRG Web background color", color); continue; }
            const std::size_t styleEnd = webPageHtml.find("</style>");
            if (styleEnd != std::string::npos) webPageHtml.insert(styleEnd, "body{background:" + color + ";}");
            continue;
        }

        if (cmd == "web_image") {
            if (!webEnabled || webPageHtml.empty() || tokens.size() != 2) { reportError(i + 1, "use web_image PROJECT_FILE after web_page"); continue; }
            bool pathValid = false;
            const auto imagePath = projectFilePath(valueFor(tokens[1], variables), pathValid);
            if (!pathValid || !std::filesystem::is_regular_file(imagePath)) { reportError(i + 1, "image must be inside the TRG project folder", tokens[1]); continue; }
            std::ifstream imageFile(imagePath, std::ios::binary);
            std::ostringstream bytes; bytes << imageFile.rdbuf();
            const std::string extension = lower(imagePath.extension().u8string());
            const std::string mime = extension == ".png" ? "image/png" : extension == ".gif" ? "image/gif" :
                extension == ".webp" ? "image/webp" : "image/jpeg";
            webPageHtml += "<img style=\"max-width:100%;height:auto\" src=\"data:" + mime + ";base64," + base64Encode(bytes.str()) + "\">";
            continue;
        }

        if (cmd == "web_show") {
            if (!webEnabled || webPageHtml.empty()) { reportError(i + 1, "create a TRG Web page before web_show"); continue; }
            const std::string html = webPageHtml + "</body></html>";
#ifdef __ANDROID__
            trgStudioActionAndroid("webpage:" + html);
#elif defined(TRG_WEB)
            appendOutput("TRG Web preview updated.\r\n");
#elif defined(_WIN32)
            const std::filesystem::path previewPath = std::filesystem::temp_directory_path() / "trg_web_preview.html";
            { std::ofstream previewFile(previewPath, std::ios::binary); previewFile << html; }
            const HINSTANCE opened = ShellExecuteW(mainWindow, L"open", previewPath.wstring().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
            if (reinterpret_cast<INT_PTR>(opened) <= 32) reportError(i + 1, "could not open the TRG Web preview");
#else
            appendOutput("TRG Web preview is available in TRG Studio.\r\n");
#endif
            continue;
        }

        if (cmd == "run_wasm") {
            if (!webEnabled) { reportError(i + 1, "enable WebAssembly with add (web)"); continue; }
            if (tokens.size() != 2) { reportError(i + 1, "use run_wasm PROJECT_FILE"); continue; }
            bool pathValid = false;
            const std::filesystem::path wasmPath = projectFilePath(valueFor(tokens[1], variables), pathValid);
            if (!pathValid || !std::filesystem::is_regular_file(wasmPath) || lower(wasmPath.extension().u8string()) != ".wasm") {
                reportError(i + 1, "run_wasm needs an existing .wasm file inside the TRG project folder", tokens[1]);
                continue;
            }
#ifdef __ANDROID__
            trgStudioActionAndroid("wasm:" + wasmPath.u8string());
#elif defined(_WIN32)
            std::ifstream moduleFile(wasmPath, std::ios::binary);
            std::ostringstream moduleBytes; moduleBytes << moduleFile.rdbuf();
            const std::string encoded = base64Encode(moduleBytes.str());
            const std::string html = "<!doctype html><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\"><body style=\"background:#20202a;color:#fff;font:16px sans-serif;padding:24px\"><h2>TRG WebAssembly</h2><p>" +
                htmlEscape(wasmPath.filename().u8string()) + "</p><pre id=\"status\">Loading module…</pre><script>try{const b=Uint8Array.from(atob('" + encoded +
                "'),c=>c.charCodeAt(0));WebAssembly.instantiate(b,{env:{}}).then(r=>{window.trgWasm=r.instance;document.getElementById('status').textContent='Module loaded and its start function ran.';}).catch(e=>document.getElementById('status').textContent='Module error: '+e)}catch(e){document.getElementById('status').textContent='Module error: '+e}</script></body>";
            const auto previewPath = std::filesystem::temp_directory_path() / "trg_wasm_preview.html";
            { std::ofstream previewFile(previewPath, std::ios::binary); previewFile << html; }
            const HINSTANCE opened = ShellExecuteW(mainWindow, L"open", previewPath.wstring().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
            if (reinterpret_cast<INT_PTR>(opened) <= 32) reportError(i + 1, "could not open the WebAssembly preview");
#else
            reportError(i + 1, "WebAssembly preview is not supported on this platform", tokens[1]);
#endif
            continue;
        }

        
        if (cmd == "word" || cmd == "num" || cmd == "toggle" || cmd == "color") {
            if (tokens.size() < 2 || !validName(tokens[1])) {
                reportError(i + 1, "invalid declaration", tokens.size() > 1 ? tokens[1] : "");
                continue;
            }
            const std::string name = tokens[1];
            std::string rawValue;
            const std::size_t namePos = positionAfterArgument(line, name);
            if (namePos == std::string::npos) { reportError(i + 1, "invalid declaration", name); continue; }
            rawValue = trim(line.substr(namePos));
            if (!rawValue.empty() && rawValue[0] == '=') rawValue = trim(rawValue.substr(1));
            else if (!rawValue.empty()) { reportError(i + 1, "expected '=' before value", name); continue; }

            std::string value = defaults[cmd];
            if (!rawValue.empty()) {
                const std::vector<std::string> valueTokens = tokenize(rawValue);
                if (valueTokens.size() == 1) value = valueFor(valueTokens[0], variables);
                else value = rawValue;
                if (cmd == "word" && rawValue.front() == '"') value = valueTokens.empty() ? "" : valueTokens[0];
                if (cmd == "num") {
                    double numeric = 0;
                    if (!evaluateExpression(rawValue, variables, numeric)) { reportError(i + 1, "invalid number", rawValue); continue; }
                    value = numberString(numeric);
                } else if (cmd == "toggle") {
                    value = lower(value);
                    if (value != "true" && value != "false") { reportError(i + 1, "toggle must be true or false", value); continue; }
                } else if (cmd == "color") {
                    unsigned red = 0, green = 0, blue = 0;
                    if (!parseColor(value, red, green, blue)) { reportError(i + 1, "color must be #RRGGBB or 0xRRGGBB", value); continue; }
                }
            }
            if (cmd == "num") { double numeric = 0; parseNumber(value, numeric); variables[name] = numeric; }
            else if (cmd == "toggle") variables[name] = value == "true";
            else variables[name] = value;
            types[name] = cmd;
            continue;
        }

        
        if (cmd == "default") {
            if (tokens.size() < 3 || defaults.count(tokens[1]) == 0) {
                reportError(i + 1, "use default TYPE VALUE");
                continue;
            }
            const std::size_t typePos = positionAfterArgument(line, tokens[1]);
            if (typePos == std::string::npos) { reportError(i + 1, "use default TYPE VALUE"); continue; }
            std::string valueText = trim(line.substr(typePos));
            if (!valueText.empty() && valueText.front() == '=') valueText = trim(valueText.substr(1));
            const std::vector<std::string> valueTokens = tokenize(valueText);
            const std::string value = valueTokens.size() == 1 ? valueFor(valueTokens[0], variables) : valueText;
            if (tokens[1] == "num") {
                double numeric = 0;
                if (!evaluateExpression(valueText, variables, numeric)) { reportError(i + 1, "invalid number", valueText); continue; }
                defaults[tokens[1]] = numberString(numeric);
            } else if (tokens[1] == "toggle") {
                const std::string toggleValue = lower(value);
                if (toggleValue != "true" && toggleValue != "false") { reportError(i + 1, "toggle must be true or false", value); continue; }
                defaults[tokens[1]] = toggleValue;
            } else if (tokens[1] == "color") {
                unsigned red = 0, green = 0, blue = 0;
                if (!parseColor(value, red, green, blue)) { reportError(i + 1, "invalid color", value); continue; }
                defaults[tokens[1]] = value;
            } else {
                defaults[tokens[1]] = value;
            }
            continue;
        }

        
        if (cmd == "group" || cmd == "if" || cmd == "loop" || cmd == "every") {
            std::size_t open = 0, close = 0;
            if (!findBlock(program, i, end, open, close)) { reportError(i + 1, "missing block"); continue; }
            if (cmd == "group") {
                const bool namedGroup = tokens.size() >= 2 && validName(tokens[1]);
                const std::string groupName = namedGroup ? tokens[1] : "";
                std::unordered_set<std::string> before;
                for (const auto& item : variables) before.insert(item.first);
                const std::string previousGroup = currentUiGroup;
                Defaults localDefaults = defaults;
                if (namedGroup && uiMode) {
                    bool initiallyVisible = false;
                    for (std::size_t part = 2; part + 1 < tokens.size(); part += 2)
                        if (lower(tokens[part]) == "visible") initiallyVisible = lower(valueFor(tokens[part + 1], variables)) == "true";
                    auto group = std::find_if(uiGroups.begin(), uiGroups.end(), [&](const UiGroup& item) { return item.name == groupName; });
                    const std::string parentGroup = currentUiGroup == groupName ? "default" : currentUiGroup;
                    if (group == uiGroups.end()) uiGroups.push_back({groupName, initiallyVisible, parentGroup});
                    else { group->visible = initiallyVisible; group->parent = parentGroup; }
                    currentUiGroup = groupName;
                }
                if (!groupName.empty()) { groupStack.push_back(groupName); groupSnapshots.push_back(before); }
                const std::vector<std::string> membership = groupStack;
                Flow inner;
                executeBlock(program, open + 1, close, variables, types, localDefaults, inner, manualMode,
                             namedGroup && uiMode ? true : uiMode);
                if (!groupName.empty()) { groupStack.pop_back(); groupSnapshots.pop_back(); }
                currentUiGroup = previousGroup;
                if (!groupName.empty()) {
                    
                    
                    for (const auto& item : variables) {
                        if (before.count(item.first) || variableGroups.count(item.first)) continue;
                        variableGroups[item.first] = membership;
                    }
                }
                if (inner.stopped) { flow.stopped = true; return; }
            } else if (cmd == "every") {
                long long interval = 0;
                if (tokens.size() < 2 || !parseDurationMilliseconds(valueFor(tokens[1], variables), interval) || interval <= 0) {
                    reportError(i + 1, "use every 1s ( ... ) or every 100ms ( ... )");
                    i = close;
                    continue;
                }
                EveryTask task;
                task.body.assign(program.begin() + static_cast<std::ptrdiff_t>(open + 1),
                                 program.begin() + static_cast<std::ptrdiff_t>(close));
                task.interval = std::chrono::milliseconds(interval);
                task.nextRun = std::chrono::steady_clock::now() + task.interval;
                task.manualMode = manualMode;
                task.uiMode = uiMode;
                everyTasks.push_back(task);
                appendOutput("Scheduled repeating code every " + std::to_string(interval) + " ms.\r\n");
            } else if (cmd == "if") {
                const std::size_t conditionStart = line.find_first_of(" \t");
                std::string conditionText = conditionStart == std::string::npos ? "" : trim(line.substr(conditionStart + 1));
                if (!conditionText.empty() && conditionText.back() == '(') conditionText = trim(conditionText.substr(0, conditionText.size() - 1));
                std::size_t elseLine = close + 1;
                while (elseLine < end && trim(program[elseLine]).empty()) ++elseLine;
                std::size_t elseOpen = 0, elseClose = 0;
                const std::string elseHeader = elseLine < end ? lower(trim(program[elseLine])) : std::string();
                const bool hasElse = elseLine < end && elseHeader.rfind("else", 0) == 0 &&
                    (elseHeader.size() == 4 || std::isspace(static_cast<unsigned char>(elseHeader[4])) ||
                     elseHeader[4] == '(' || elseHeader[4] == '{') &&
                    findBlock(program, elseLine, end, elseOpen, elseClose);
                const bool conditionResult = conditionIsTrue(conditionText, variables);
                if (conditionResult || hasElse) {
                    Flow inner;
                    if (conditionResult) executeBlock(program, open + 1, close, variables, types, defaults, inner, manualMode, uiMode);
                    else if (hasElse) executeBlock(program, elseOpen + 1, elseClose, variables, types, defaults, inner, manualMode, uiMode);
                    if (inner.stopped) { flow.stopped = true; return; }
                    if (inner.broke) { flow.broke = true; return; }
                    if (inner.continued) { flow.continued = true; return; }
                }
                if (hasElse) close = elseClose;
            } else {
                const std::size_t conditionStart = line.find_first_of(" \t");
                std::string condition = conditionStart == std::string::npos ? "" : trim(line.substr(conditionStart + 1));
                if (!condition.empty() && condition.back() == '(') condition = trim(condition.substr(0, condition.size() - 1));
                int guard = 0;
                while (conditionIsTrue(condition, variables)) {
                    if (++guard > 100000) { reportError(i + 1, "loop limit"); break; }
                    Flow inner;
                    executeBlock(program, open + 1, close, variables, types, defaults, inner, manualMode, uiMode);
                    if (inner.stopped) { flow.stopped = true; return; }
                    if (inner.broke) break;
#ifdef _WIN32
                    if (controlsEnabled) Sleep(10);
#elif defined(__ANDROID__)
                    if (controlsEnabled) std::this_thread::sleep_for(std::chrono::milliseconds(10));
#endif
                }
            }
            i = close;
            continue;
        }

        if (cmd == "break") { flow.broke = true; return; }
        if (cmd == "continue") { flow.continued = true; return; }
        if (cmd == "stop" || cmd == "exit") { flow.stopped = true; return; }

        
        if (cmd == "register" || cmd == "registry" || cmd == "move" || cmd == "add" ||
            cmd == "subtract" || cmd == "multiply" || cmd == "divide") {
            if (!manualMode) { reportError(i + 1, "low-level commands need add (manual)", cmd); continue; }
            if (cmd == "register" || cmd == "registry") {
                const std::string name = tokens.size() > 1 ? unbraced(tokens[1]) : "";
                if (tokens.size() < 2 || tokens.size() > 3 || !validName(name)) {
                    reportError(i + 1, "use register NAME [num|word|toggle|color]");
                    continue;
                }
                const std::string type = tokens.size() == 3 ? tokens[2] : "num";
                if (!defaults.count(type)) { reportError(i + 1, "unknown register type", type); continue; }
                if (type == "num") { double numeric = 0; parseNumber(defaults[type], numeric); variables[name] = numeric; }
                else if (type == "toggle") variables[name] = defaults[type] == "true";
                else variables[name] = defaults[type];
                types[name] = type;
                continue;
            }
            const std::string name = tokens.size() > 1 ? unbraced(tokens[1]) : "";
            if (tokens.size() != 3 || !variables.count(name)) {
                reportError(i + 1, "register instruction needs an existing register and value");
                continue;
            }
            const std::string type = types.count(name) ? types[name] : "num";
            if (cmd == "move") {
                const std::string value = valueFor(tokens[2], variables);
                if (type == "num") {
                    double numeric = 0;
                    if (!evaluateExpression(tokens[2], variables, numeric)) { reportError(i + 1, "invalid number", tokens[2]); continue; }
                    variables[name] = numeric;
                } else if (type == "toggle") {
                    const std::string toggleValue = lower(value);
                    if (toggleValue != "true" && toggleValue != "false") { reportError(i + 1, "toggle must be true or false", value); continue; }
                    variables[name] = toggleValue == "true";
                } else if (type == "color") {
                    unsigned red = 0, green = 0, blue = 0;
                    if (!parseColor(value, red, green, blue)) { reportError(i + 1, "invalid color", value); continue; }
                    variables[name] = value;
                } else variables[name] = value;
                continue;
            }
            double left = 0, right = 0;
            if (type != "num" || !parseNumber(static_cast<std::string>(variables[name]), left) ||
                !evaluateExpression(tokens[2], variables, right)) {
                reportError(i + 1, "arithmetic instructions need numeric registers and values");
                continue;
            }
            if (cmd == "add") left += right;
            else if (cmd == "subtract") left -= right;
            else if (cmd == "multiply") left *= right;
            else if (right == 0) { reportError(i + 1, "division by zero"); continue; }
            else left /= right;
            variables[name] = left;
            continue;
        }

        
        if (cmd == "text") {
            if (!uiMode || tokens.size() < 10) {
                reportError(i + 1, "use text NAME VALUE x X y Y size SIZE color COLOR [font sans|serif|mono] [align left|center|right]");
                continue;
            }
            UiText text;
            text.group = currentUiGroup;
            std::size_t firstProperty = 2;
            const bool named = tokens.size() > 10 && validName(tokens[1]) && lower(tokens[2]) != "x";
            if (named) {
                text.name = tokens[1];
                text.value = valueFor(tokens[2], variables);
                firstProperty = 3;
            } else {
                text.name = "text_" + std::to_string(uiTexts.size());
                text.value = valueFor(tokens[1], variables);
                firstProperty = 2;
            }
            unsigned red = 0, green = 0, blue = 0;
            bool validText = true;
            for (std::size_t part = firstProperty; part + 1 < tokens.size(); part += 2) {
                const std::string key = lower(tokens[part]);
                const std::string value = valueFor(tokens[part + 1], variables);
                if (key == "x") validText = parseNumber(value, text.x);
                else if (key == "y") validText = parseNumber(value, text.y);
                else if (key == "size") validText = parseNumber(value, text.size) && text.size > 0;
                else if (key == "color") { text.color = value; validText = parseColor(value, red, green, blue); }
                else if (key == "font") {
                    text.font = lower(value);
                    if (text.font == "monospace") text.font = "mono";
                    validText = text.font == "sans" || text.font == "serif" || text.font == "mono";
                } else if (key == "align") {
                    text.align = lower(value);
                    validText = text.align == "left" || text.align == "center" || text.align == "right";
                } else if (key == "bold") text.bold = lower(value) == "true" || value == "1";
                else if (key == "opacity") validText = parseNumber(value, text.opacity) && text.opacity >= 0 && text.opacity <= 1;
                else validText = false;
                if (!validText) break;
            }
            if (!validText || text.color.empty() || text.size <= 0) {
                reportError(i + 1, "invalid text position, size, color, font, alignment, or opacity", text.name);
                continue;
            }
            if (std::any_of(uiTexts.begin(), uiTexts.end(), [&](const UiText& item) { return item.name == text.name; })) {
                reportError(i + 1, "duplicate text name", text.name);
                continue;
            }
            uiTexts.push_back(std::move(text));
            continue;
        }
        if (cmd == "button") {
            if (!uiMode || tokens.size() < 3) {
                reportError(i + 1, "use button NAME LABEL x X y Y w WIDTH h HEIGHT ( ... ) inside add (ui)");
                continue;
            }
            std::size_t open = 0, close = 0;
            if (!findBlock(program, i, end, open, close)) {
                reportError(i + 1, "button needs a code block", tokens[1]);
                continue;
            }
            UiButton button;
            button.name = tokens[1];
            button.label = valueFor(tokens[2], variables);
            button.group = currentUiGroup;
            bool validButton = validName(button.name);
            for (std::size_t a = 3; validButton && a + 1 < tokens.size(); a += 2) {
                const std::string key = lower(tokens[a]);
                double number = 0;
                const std::string property = valueFor(tokens[a + 1], variables);
                if (key == "color") {
                    unsigned red = 0, green = 0, blue = 0;
                    if (!parseColor(property, red, green, blue)) { reportError(i + 1, "button color must be #RRGGBB or 0xRRGGBB", property); validButton = false; }
                    else button.color = property;
                } else if (key == "text_color") {
                    unsigned red = 0, green = 0, blue = 0;
                    if (!parseColor(property, red, green, blue)) { reportError(i + 1, "button text_color must be a color", property); validButton = false; }
                    else button.textColor = property;
                } else if (key == "font") {
                    button.font = lower(property);
                    if (button.font == "monospace") button.font = "mono";
                    if (button.font != "sans" && button.font != "serif" && button.font != "mono") validButton = false;
                } else if (key == "bold") {
                    button.textBold = lower(property) == "true" || property == "1";
                } else if (key == "text_size") {
                    if (!parseNumber(property, number) || number <= 0 || number > 1) validButton = false;
                    else button.textSize = number;
                } else if (key == "opacity") {
                    if (!parseNumber(property, number) || number < 0 || number > 1) validButton = false;
                    else button.opacity = number;
                } else if (key == "edges" || key == "shape") {
                    const std::string edge = lower(property);
                    if (edge == "round" || edge == "rounded") button.cornerRadius = 7;
                    else if (edge == "square") button.cornerRadius = 0;
                    else { reportError(i + 1, "button edges must be round or square", property); validButton = false; }
                } else if (key == "radius") {
                    if (!parseNumber(property, number) || number < 0) { reportError(i + 1, "button radius must be zero or more", property); validButton = false; }
                    else button.cornerRadius = number;
                } else if (!parseNumber(property, number)) {
                    reportError(i + 1, "invalid button coordinate", tokens[a + 1]);
                    validButton = false;
                } else if (key == "x") button.x = number;
                else if (key == "y") button.y = number;
                else if (key == "w") button.width = number;
                else if (key == "h") button.height = number;
                else if (key == "opacity") button.opacity = number;
                else {
                    reportError(i + 1, "unknown button property", key);
                    validButton = false;
                }
            }
            if (!validButton || button.width <= 0 || button.height <= 0 || button.opacity < 0 || button.opacity > 1) {
                if (validButton && (button.width <= 0 || button.height <= 0)) reportError(i + 1, "button size must be positive", button.name);
                else if (validButton) reportError(i + 1, "button opacity must be between 0 and 1", button.name);
                i = close;
                continue;
            }
            if (std::any_of(uiButtons.begin(), uiButtons.end(), [&](const UiButton& existing) { return existing.name == button.name; })) {
                reportError(i + 1, "duplicate button name", button.name);
                i = close;
                continue;
            }
            button.body.assign(program.begin() + static_cast<std::ptrdiff_t>(open + 1),
                               program.begin() + static_cast<std::ptrdiff_t>(close));
            uiButtons.push_back(std::move(button));
            i = close;
            continue;
        }
        if (cmd == "block") {
            if (!uiMode) { reportError(i + 1, "block needs add (ui)"); continue; }
            UiBlock block;
            block.group = currentUiGroup;
            for (std::size_t a = 1; a + 1 < tokens.size(); a += 2) {
                const std::string key = lower(tokens[a]);
                const std::string value = valueFor(tokens[a + 1], variables);
                double number = 0;
                if (key == "name") block.name = tokens[a + 1];
                else if (key == "color") block.color = value;
                else if (key == "opacity") {
                    if (!parseNumber(value, number) || number < 0 || number > 1) { reportError(i + 1, "block opacity must be between 0 and 1", value); block.name.clear(); break; }
                    block.opacity = number;
                }
                else if (key == "edges" || key == "shape") {
                    const std::string edge = lower(value);
                    if (key == "shape") {
                        if (edge == "square" || edge == "rect") block.shape = "square";
                        else if (edge == "circle" || edge == "round") block.shape = "circle";
                        else if (edge == "triangle") block.shape = "triangle";
                        else if (edge == "pentagon") block.shape = "pentagon";
                        else if (edge == "hexagon") block.shape = "hexagon";
                        else if (edge == "star") block.shape = "star";
                        else { reportError(i + 1, "block shape must be square, circle, triangle, pentagon, hexagon, or star", value); block.name.clear(); break; }
                        block.cornerRadius = 0;
                    } else {
                        if (edge == "round" || edge == "rounded") block.cornerRadius = 10;
                        else if (edge == "square") block.cornerRadius = 0;
                        else { reportError(i + 1, "block edges must be round or square", value); block.name.clear(); break; }
                    }
                } else if (key == "glow") {
                    // glow COLOR INTENSITY
                    block.glowColor = value;
                    if (a + 3 < tokens.size()) {
                        if (!parseNumber(tokens[a + 3], number)) { reportError(i + 1, "glow needs intensity number", tokens[a + 3]); block.name.clear(); break; }
                        block.glowIntensity = number;
                        a += 2; // skip intensity value
                    } else {
                        block.glowIntensity = 0.5;
                    }
                } else if (key == "radius") {
                    if (!parseNumber(value, number) || number < 0) { reportError(i + 1, "block radius must be zero or more", value); block.name.clear(); break; }
                    block.cornerRadius = number;
                } else if (key == "z" || key == "x" || key == "y" || key == "h" || key == "w") {
                    if (!parseNumber(value, number)) { reportError(i + 1, "invalid block coordinate", value); block.name.clear(); break; }
                    if (key == "z") block.z = static_cast<int>(number);
                    else if (key == "x") block.x = number;
                    else if (key == "y") block.y = number;
                    else if (key == "h") block.height = number;
                    else block.width = number;
                } else { reportError(i + 1, "unknown block property", key); block.name.clear(); break; }
            }
            unsigned red = 0, green = 0, blue = 0;
            if (block.name.empty() || !parseColor(block.color, red, green, blue)) {
                if (!block.name.empty()) reportError(i + 1, "block color must be a color variable or #RRGGBB value", block.color);
                continue;
            }
            const auto existing = std::find_if(uiBlocks.begin(), uiBlocks.end(), [&](const UiBlock& item) { return item.name == block.name; });
            if (existing != uiBlocks.end()) { reportError(i + 1, "duplicate block name", block.name); continue; }
            uiBlocks.push_back(block);
            continue;
        }
        if (cmd == "image" || cmd == "sprite") {
            if (!uiMode || (tokens.size() != 11 && tokens.size() != 13) || !validName(tokens[1])) {
                reportError(i + 1, "use sprite NAME FILE x X y Y w WIDTH h HEIGHT [opacity 0..1] inside add (ui)");
                continue;
            }
            UiSprite sprite;
            sprite.name = tokens[1];
            sprite.group = currentUiGroup;
            sprite.path = valueFor(tokens[2], variables);
            double* values[] = {&sprite.x, &sprite.y, &sprite.width, &sprite.height};
            const char* keys[] = {"x", "y", "w", "h"};
            bool valid = true;
            for (std::size_t field = 0; field < 4; ++field) {
                double number = 0;
                const std::size_t keyIndex = 3 + field * 2;
                if (lower(tokens[keyIndex]) != keys[field] || !parseNumber(valueFor(tokens[keyIndex + 1], variables), number)) {
                    valid = false; break;
                }
                *values[field] = number;
            }
            if (valid && tokens.size() == 13) {
                double alpha = 0;
                valid = lower(tokens[11]) == "opacity" && parseNumber(valueFor(tokens[12], variables), alpha) && alpha >= 0 && alpha <= 1;
                if (valid) sprite.opacity = alpha;
            }
            bool pathValid = false;
            const auto imagePath = projectFilePath(sprite.path, pathValid);
            if (!valid || sprite.width <= 0 || sprite.height <= 0 || !pathValid || !std::filesystem::is_regular_file(imagePath)) {
                reportError(i + 1, "sprite needs a project image and valid x/y/w/h values", sprite.path);
                continue;
            }
            sprite.path = imagePath.u8string();
            uiSprites.push_back(std::move(sprite));
            continue;
        }
        if (cmd == "animate") {
            if (!uiMode || tokens.size() != 10) { reportError(i + 1, "use animate NAME x DX y DY fps RATE time SECONDS"); continue; }
            auto block = std::find_if(uiBlocks.begin(), uiBlocks.end(), [&](const UiBlock& item) { return item.name == tokens[1]; });
            if (block == uiBlocks.end()) { reportError(i + 1, "unknown block", tokens[1]); continue; }
            double dx = 0, dy = 0, fps = 0, duration = 0;
            if (lower(tokens[2]) != "x" || lower(tokens[4]) != "y" ||
                lower(tokens[6]) != "fps" || lower(tokens[8]) != "time" ||
                !parseNumber(tokens[3], dx) || !parseNumber(tokens[5], dy) ||
                !parseNumber(tokens[7], fps)) {
                reportError(i + 1, "invalid animation");
                continue;
            }
            std::string durationText = tokens[9];
            if (!durationText.empty() && (durationText.back() == 's' || durationText.back() == 'S')) durationText.pop_back();
            if (!parseNumber(durationText, duration) || duration <= 0 || fps <= 0) {
                reportError(i + 1, "animation time and fps must be positive");
                continue;
            }
            UiAnimation animation;
            animation.name = block->name;
            animation.startTime = std::chrono::duration<double>(std::chrono::steady_clock::now() - uiStart).count();
            animation.startX = block->x;
            animation.startY = block->y;
            for (const UiAnimation& previous : uiAnimations) {
                if (previous.name == block->name) {
                    animation.startTime = std::max(animation.startTime, previous.startTime + previous.duration);
                    animation.startX = previous.startX + previous.dx;
                    animation.startY = previous.startY + previous.dy;
                }
            }
            animation.dx = dx;
            animation.dy = dy;
            animation.duration = duration;
            animation.fps = static_cast<int>(std::min(fps, 1000.0));
            uiAnimations.push_back(animation);
            continue;
        }

        
        const std::size_t eqPos = line.find('=');
        if (eqPos != std::string::npos) {
            const std::string name = trim(line.substr(0, eqPos));
            const std::string rawValue = trim(line.substr(eqPos + 1));
            if (validVariablePath(name) && variables.count(name)) {
                const std::string type = types.count(name) ? types[name] : "word";
                const std::string value = valueFor(rawValue, variables);
                if (type == "num") {
                    double numeric = 0;
                    if (!evaluateExpression(rawValue, variables, numeric)) { reportError(i + 1, "invalid number", rawValue); continue; }
                    variables[name] = numeric;
                } else if (type == "toggle") {
                    if (lower(value) != "true" && lower(value) != "false") { reportError(i + 1, "toggle must be true or false", value); continue; }
                    variables[name] = lower(value) == "true";
                } else if (type == "color") {
                    unsigned red = 0, green = 0, blue = 0;
                    if (!parseColor(value, red, green, blue)) { reportError(i + 1, "invalid color", value); continue; }
                    variables[name] = value;
                } else {
                    const std::vector<std::string> valueTokens = tokenize(rawValue);
                    if (valueTokens.empty()) variables[name].clear();
                    else if (valueTokens.size() == 1) variables[name] = valueFor(valueTokens[0], variables);
                    else variables[name] = rawValue;
                }
                continue;
            }
        }

        
        const std::string suggestion = suggestedCommand(cmd);
        reportError(i + 1, suggestion.empty() ? "unknown command" : "unknown command; did you mean '" + suggestion + "'?", cmd);
    }
}

bool uiEntityBounds(const std::string& name, double& x, double& y, double& width, double& height) {
    for (const UiBlock& block : uiBlocks) {
        if (block.name != name || !block.visible || !uiGroupIsVisible(block.group)) continue;
        x = block.x; y = block.y; width = block.width; height = block.height;
        const double elapsed = std::chrono::duration<double>(std::chrono::steady_clock::now() - uiStart).count();
        for (const UiAnimation& animation : uiAnimations) {
            if (animation.name != name) continue;
            if (elapsed < animation.startTime) break;
            const double progress = std::min(1.0, (elapsed - animation.startTime) / animation.duration);
            x = animation.startX + animation.dx * progress;
            y = animation.startY + animation.dy * progress;
        }
        return true;
    }
    for (const UiSprite& sprite : uiSprites) {
        if (sprite.name == name && sprite.visible && uiGroupIsVisible(sprite.group)) { x = sprite.x; y = sprite.y; width = sprite.width; height = sprite.height; return true; }
    }
    for (const UiButton& button : uiButtons) {
        if (button.name == name && button.visible && uiGroupIsVisible(button.group)) { x = button.x; y = button.y; width = button.width; height = button.height; return true; }
    }
    for (const UiText& text : uiTexts) {
        if (text.name == name && text.visible && uiGroupIsVisible(text.group)) { x = text.x; y = text.y; width = text.size; height = text.size; return true; }
    }
    return false;
}

void refreshCollisionWatches(Vars& variables, Types& currentTypes) {
    for (const CollisionWatch& watch : collisionWatches) {
        double ax = 0, ay = 0, aw = 0, ah = 0, bx = 0, by = 0, bw = 0, bh = 0;
        const bool hasA = uiEntityBounds(watch.first, ax, ay, aw, ah);
        const bool hasB = uiEntityBounds(watch.second, bx, by, bw, bh);
        const bool overlap = hasA && hasB && ax < bx + bw && ax + aw > bx &&
            ay < by + bh && ay + ah > by;
        variables[watch.result] = overlap;
        if (!currentTypes.count(watch.result)) currentTypes[watch.result] = "toggle";
    }
}

#ifdef _WIN32
std::pair<double, double> animatedPosition(const UiBlock& block, double elapsed) {
    double x = block.x;
    double y = block.y;
    for (const UiAnimation& animation : uiAnimations) {
        if (animation.name != block.name) continue;
        if (elapsed < animation.startTime) break;
        const double progress = std::min(1.0, (elapsed - animation.startTime) / animation.duration);
        x = animation.startX + animation.dx * progress;
        y = animation.startY + animation.dy * progress;
    }
    return {x, y};
}

struct DesktopLayout {
    LONG width;
    LONG height;
    LONG headerHeight;
    LONG panelTop;
    LONG canvasHeight;
    LONG margin;
    LONG outputTop;
    LONG outputBottom;
};

DesktopLayout desktopLayout(HWND window, const RECT& client) {
    const LONG width = std::max<LONG>(0, client.right - client.left);
    const LONG height = std::max<LONG>(0, client.bottom - client.top);
    const LONG headerHeight = std::min<LONG>(scaleForDpi(window, 76),
        std::max<LONG>(scaleForDpi(window, 48), height / 5));
    const LONG margin = scaleForDpi(window, 20);
    const LONG outputMinimum = scaleForDpi(window, 80);
    const LONG canvasMinimum = scaleForDpi(window, 100);
    const LONG panelMinimum = std::min(height, headerHeight + canvasMinimum);
    const LONG panelMaximum = std::max(panelMinimum, height - outputMinimum);
    const LONG panelTop = std::clamp<LONG>(height * 65 / 100, panelMinimum, panelMaximum);
    return {
        width,
        height,
        headerHeight,
        panelTop,
        std::max<LONG>(1, panelTop - headerHeight),
        margin,
        panelTop + scaleForDpi(window, 10),
        std::max<LONG>(panelTop + scaleForDpi(window, 10),
                       height - scaleForDpi(window, 16))
    };
}

LRESULT CALLBACK mainWindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_CREATE) {
        mainWindow = window;
        openButton = CreateWindowExW(0, L"BUTTON", L"Open",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 100, 32, window,
            reinterpret_cast<HMENU>(openButtonId), GetModuleHandleW(nullptr), nullptr);
        saveButton = CreateWindowExW(0, L"BUTTON", L"Save",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 100, 32, window,
            reinterpret_cast<HMENU>(saveButtonId), GetModuleHandleW(nullptr), nullptr);
        runButton = CreateWindowExW(0, L"BUTTON", L"Run",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 100, 32, window,
            reinterpret_cast<HMENU>(runButtonId), GetModuleHandleW(nullptr), nullptr);
        previewButton = CreateWindowExW(0, L"BUTTON", L"Preview",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 100, 32, window,
            reinterpret_cast<HMENU>(previewButtonId), GetModuleHandleW(nullptr), nullptr);
        newButton = CreateWindowExW(0, L"BUTTON", L"New",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 100, 32, window,
            reinterpret_cast<HMENU>(newButtonId), GetModuleHandleW(nullptr), nullptr);
        codeEditor = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL | WS_HSCROLL |
                ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_WANTRETURN | ES_NOHIDESEL,
            0, 0, 100, 100, window, reinterpret_cast<HMENU>(codeEditorId),
            GetModuleHandleW(nullptr), nullptr);
        SendMessageW(codeEditor, EM_SETLIMITTEXT, 8 * 1024 * 1024, 0);
        setOpenButtonFont(window);
        layoutOutputControls(window);
        updateEditorWindowTitle();
        return 0;
    }
    if (message == WM_SIZE) {
        layoutOutputControls(window);
        InvalidateRect(window, nullptr, TRUE);
        return 0;
    }
    if (message == WM_GETMINMAXINFO) {
        auto* limits = reinterpret_cast<MINMAXINFO*>(lParam);
        limits->ptMinTrackSize.x = scaleForDpi(window, 640);
        limits->ptMinTrackSize.y = scaleForDpi(window, 420);
        return 0;
    }
    if (message == WM_DPICHANGED) {
        const RECT* suggested = reinterpret_cast<const RECT*>(lParam);
        SetWindowPos(window, nullptr, suggested->left, suggested->top,
                     suggested->right - suggested->left, suggested->bottom - suggested->top,
                     SWP_NOZORDER | SWP_NOACTIVATE);
        setOpenButtonFont(window);
        layoutOutputControls(window);
        InvalidateRect(window, nullptr, TRUE);
        return 0;
    }
    if (message == WM_COMMAND && LOWORD(wParam) == codeEditorId && HIWORD(wParam) == EN_CHANGE) {
        if (!loadingEditorText) {
            editorDirty = true;
            updateEditorWindowTitle();
        }
        return 0;
    }
    if (message == WM_COMMAND && LOWORD(wParam) == openButtonId) {
        std::filesystem::path path;
        if (openSourceDialog(path, window) && confirmDiscardEditorChanges(window)) loadSourceFile(path);
        return 0;
    }
    if (message == WM_COMMAND && LOWORD(wParam) == saveButtonId) {
        saveSourceFile();
        return 0;
    }
    if (message == WM_COMMAND && LOWORD(wParam) == runButtonId) {
        runEditorSource();
        return 0;
    }
    if (message == WM_COMMAND && LOWORD(wParam) == previewButtonId) {
        previewMode = !previewMode;
        SetWindowTextW(previewButton, previewMode ? L"Code" : L"Preview");
        layoutOutputControls(window);
        InvalidateRect(window, nullptr, TRUE);
        return 0;
    }
    if (message == WM_COMMAND && LOWORD(wParam) == newButtonId) {
        if (confirmDiscardEditorChanges(window)) {
            currentSourcePath.clear();
            editorDirty = false;
            previewMode = false;
            loadingEditorText = true;
            SetWindowTextW(codeEditor, L"");
            loadingEditorText = false;
            SetWindowTextW(previewButton, L"Preview");
            outputBuffer.clear();
            projectRoot = std::filesystem::current_path();
            uiBlocks.clear(); uiSprites.clear(); uiAnimations.clear();
            everyTasks.clear(); uiButtons.clear(); uiTexts.clear();
            collisionWatches.clear();
            updateEditorWindowTitle();
            layoutOutputControls(window);
            InvalidateRect(window, nullptr, TRUE);
        }
        return 0;
    }
    if (message == WM_LBUTTONUP && !uiButtons.empty()) {
        RECT client = {};
        GetClientRect(window, &client);
        const DesktopLayout layout = desktopLayout(window, client);
        const double canvasWidth = static_cast<double>(std::max<LONG>(1, layout.width));
        const double canvasHeight = static_cast<double>(layout.canvasHeight);
        const LONG mouseX = static_cast<short>(LOWORD(lParam));
        const LONG mouseY = static_cast<short>(HIWORD(lParam));
        for (auto button = uiButtons.rbegin(); button != uiButtons.rend(); ++button) {
            if (!button->visible || !uiGroupIsVisible(button->group)) continue;
            RECT area = {
                static_cast<LONG>(button->x * canvasWidth / 100.0),
                static_cast<LONG>(layout.headerHeight + button->y * canvasHeight / 100.0),
                static_cast<LONG>((button->x + button->width) * canvasWidth / 100.0),
                static_cast<LONG>(layout.headerHeight +
                    (button->y + button->height) * canvasHeight / 100.0)
            };
            if (PtInRect(&area, POINT{mouseX, mouseY})) {
                outputBuffer.clear();
                appendOutput("--- " + button->label + " ---\r\n");
                conditionIsTrue("false", uiVariables);
                Flow flow;
                executeBlock(button->body, 0, button->body.size(), uiVariables, uiTypes,
                             uiDefaults, flow, false, true);
                refreshCollisionWatches(uiVariables, uiTypes);
                if (flow.stopped) everyTasks.clear();
                if (!uiAnimations.empty() || !everyTasks.empty()) SetTimer(window, 1, 16, nullptr);
                InvalidateRect(window, nullptr, TRUE);
                return 0;
            }
        }
    }
    if (message == WM_TIMER) {
        const double elapsed = std::chrono::duration<double>(std::chrono::steady_clock::now() - uiStart).count();
        const auto now = std::chrono::steady_clock::now();
        for (std::size_t index = 0; index < everyTasks.size();) {
            if (now < everyTasks[index].nextRun) { ++index; continue; }
            const EveryTask task = everyTasks[index];
            everyTasks[index].nextRun = now + task.interval;
            refreshCollisionWatches(uiVariables, uiTypes);
            Flow taskFlow;
            executeBlock(task.body, 0, task.body.size(), uiVariables, uiTypes,
                         uiDefaults, taskFlow, task.manualMode, task.uiMode);
            if (taskFlow.stopped) { everyTasks.clear(); break; }
            if (taskFlow.broke) everyTasks.erase(everyTasks.begin() + static_cast<std::ptrdiff_t>(index));
            else ++index;
        }
        const bool animationsFinished = std::all_of(uiAnimations.begin(), uiAnimations.end(),
            [&](const UiAnimation& animation) { return elapsed >= animation.startTime + animation.duration; });
        InvalidateRect(window, nullptr, FALSE);
        if (animationsFinished && everyTasks.empty()) KillTimer(window, 1);
        return 0;
    }
    if (message == WM_PAINT) {
        PAINTSTRUCT paint;
        HDC dc = BeginPaint(window, &paint);
        RECT client;
        GetClientRect(window, &client);
        HBRUSH background = CreateSolidBrush(RGB(24, 22, 32));
        FillRect(dc, &client, background);
        DeleteObject(background);
        const DesktopLayout layout = desktopLayout(window, client);
        RECT header = {0, 0, layout.width, layout.headerHeight};
        HBRUSH purple = CreateSolidBrush(RGB(91, 52, 170));
        FillRect(dc, &header, purple);
        DeleteObject(purple);
        SetBkMode(dc, TRANSPARENT);
        SetTextColor(dc, RGB(255, 255, 255));
        HFONT titleFont = CreateFontW(-scaleForDpi(window, 32), 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
            DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
        HGDIOBJ previousFont = SelectObject(dc, titleFont);
        RECT title = {
            scaleForDpi(window, 24), scaleForDpi(window, 10),
            scaleForDpi(window, 170), layout.headerHeight - scaleForDpi(window, 8)
        };
        DrawTextW(dc, L"TRG", -1, &title, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
        SelectObject(dc, previousFont);
        DeleteObject(titleFont);
        const LONG panelTop = layout.panelTop;
        RECT canvas = {0, layout.headerHeight, layout.width, panelTop};
        HBRUSH canvasBrush = CreateSolidBrush(RGB(38, 36, 44));
        FillRect(dc, &canvas, canvasBrush);
        DeleteObject(canvasBrush);
        HFONT bodyFont = CreateFontW(-scaleForDpi(window, 14), 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
            DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
        HGDIOBJ previousBodyFont = bodyFont ? SelectObject(dc, bodyFont) : nullptr;
        const double elapsed = std::chrono::duration<double>(std::chrono::steady_clock::now() - uiStart).count();
        // Update particles
        for (auto it = particles.begin(); it != particles.end();) {
            it->x += it->vx;
            it->y += it->vy;
            it->life -= 0.016;
            if (it->life <= 0) it = particles.erase(it);
            else ++it;
        }
        std::vector<const UiBlock*> ordered;
        for (const UiBlock& block : uiBlocks) ordered.push_back(&block);
        std::stable_sort(ordered.begin(), ordered.end(), [](const UiBlock* a, const UiBlock* b) { return a->z < b->z; });
        const double canvasWidth = static_cast<double>(std::max<LONG>(1, layout.width));
        const double canvasHeight = static_cast<double>(layout.canvasHeight);
        for (const UiBlock* block : ordered) {
            if (!block->visible || !uiGroupIsVisible(block->group)) continue;
            unsigned red = 0, green = 0, blue = 0;
            if (!parseColor(block->color, red, green, blue)) continue;
            const auto position = animatedPosition(*block, elapsed);
            // Apply viewport scroll offset
            double bx = position.first - viewportX;
            double by = position.second - viewportY;
            RECT area = {
                static_cast<LONG>(bx * canvasWidth / 100.0),
                static_cast<LONG>(layout.headerHeight + by * canvasHeight / 100.0),
                static_cast<LONG>((bx + block->width) * canvasWidth / 100.0),
                static_cast<LONG>(layout.headerHeight + (by + block->height) * canvasHeight / 100.0)
            };
            HBRUSH brush = CreateSolidBrush(RGB(red, green, blue));
            HPEN pen = CreatePen(PS_SOLID, 1, RGB(red, green, blue));
            HGDIOBJ oldPen = SelectObject(dc, pen);
            HGDIOBJ oldBrush = SelectObject(dc, brush);
            const int centerX = (area.left + area.right) / 2;
            const int centerY = (area.top + area.bottom) / 2;
            const int radius = std::max(1, static_cast<int>(std::min(block->width, block->height) * std::min(canvasWidth, canvasHeight) / 200.0));
            if (block->shape == "circle") {
                Ellipse(dc, area.left, area.top, area.right, area.bottom);
            } else if (block->shape == "triangle") {
                POINT tri[3] = { {centerX, area.top}, {area.left, area.bottom}, {area.right, area.bottom} };
                Polygon(dc, tri, 3);
            } else if (block->shape == "pentagon") {
                POINT pent[5];
                for (int k = 0; k < 5; ++k) {
                    double angle = -90.0 + k * 72.0;
                    double rad = angle * 3.14159265358979 / 180.0;
                    pent[k].x = centerX + static_cast<LONG>(radius * cos(rad));
                    pent[k].y = centerY + static_cast<LONG>(radius * sin(rad));
                }
                Polygon(dc, pent, 5);
            } else if (block->shape == "hexagon") {
                POINT hex[6];
                for (int k = 0; k < 6; ++k) {
                    double angle = -90.0 + k * 60.0;
                    double rad = angle * 3.14159265358979 / 180.0;
                    hex[k].x = centerX + static_cast<LONG>(radius * cos(rad));
                    hex[k].y = centerY + static_cast<LONG>(radius * sin(rad));
                }
                Polygon(dc, hex, 6);
            } else if (block->shape == "star") {
                POINT star[10];
                for (int k = 0; k < 10; ++k) {
                    double angle = -90.0 + k * 36.0;
                    double rad = angle * 3.14159265358979 / 180.0;
                    double r = (k % 2 == 0) ? radius : radius * 0.4;
                    star[k].x = centerX + static_cast<LONG>(r * cos(rad));
                    star[k].y = centerY + static_cast<LONG>(r * sin(rad));
                }
                Polygon(dc, star, 10);
            } else {
                if (block->cornerRadius > 0) {
                    const int rad = std::max(1, static_cast<int>(block->cornerRadius * std::min(canvasWidth, canvasHeight) / 100.0));
                    RoundRect(dc, area.left, area.top, area.right, area.bottom, rad * 2, rad * 2);
                } else {
                    Rectangle(dc, area.left, area.top, area.right, area.bottom);
                }
            }
            SelectObject(dc, oldBrush); SelectObject(dc, oldPen);
            DeleteObject(pen); DeleteObject(brush);
            // Draw glow
            if (block->glowIntensity > 0 && !block->glowColor.empty()) {
                unsigned gr = 0, gg = 0, gb = 0;
                if (parseColor(block->glowColor, gr, gg, gb)) {
                    int glowPad = static_cast<int>(block->glowIntensity * 12.0);
                    RECT glowArea = { area.left - glowPad, area.top - glowPad, area.right + glowPad, area.bottom + glowPad };
                    HBRUSH glowBrush = CreateSolidBrush(RGB(gr, gg, gb));
                    HPEN glowPen = CreatePen(PS_SOLID, 1, RGB(gr, gg, gb));
                    HGDIOBJ oldPen2 = SelectObject(dc, glowPen);
                    HGDIOBJ oldBrush2 = SelectObject(dc, glowBrush);
                    if (block->shape == "circle") {
                        Ellipse(dc, glowArea.left, glowArea.top, glowArea.right, glowArea.bottom);
                    } else if (block->shape == "square" || block->shape == "rect") {
                        RoundRect(dc, glowArea.left, glowArea.top, glowArea.right, glowArea.bottom, glowPad * 2, glowPad * 2);
                    } else {
                        Rectangle(dc, glowArea.left, glowArea.top, glowArea.right, glowArea.bottom);
                    }
                    SelectObject(dc, oldBrush2); SelectObject(dc, oldPen2);
                    DeleteObject(glowPen); DeleteObject(glowBrush);
                }
            }
            SetBkMode(dc, TRANSPARENT);
            SetTextColor(dc, RGB(255, 255, 255));
            const std::wstring name = wideFromUtf8(block->name);
            DrawTextW(dc, name.c_str(), -1, &area, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
        // Render particles
        if (gdiplusToken != 0) {
            Gdiplus::Graphics graphics(dc);
            for (const Particle& p : particles) {
                unsigned pr = 0, pg = 0, pb = 0;
                if (!parseColor(p.color, pr, pg, pb)) continue;
                Gdiplus::Color gcolor(static_cast<BYTE>(255 * (p.life / p.maxLife)), pr, pg, pb);
                Gdiplus::SolidBrush brush(gcolor);
                double px = p.x - viewportX;
                double py = p.y - viewportY;
                Gdiplus::RectF rect(
                    static_cast<Gdiplus::REAL>(px * canvasWidth / 100.0),
                    static_cast<Gdiplus::REAL>(layout.headerHeight + py * canvasHeight / 100.0),
                    static_cast<Gdiplus::REAL>(p.size * canvasWidth / 100.0),
                    static_cast<Gdiplus::REAL>(p.size * canvasHeight / 100.0));
                graphics.FillEllipse(&brush, rect);
            }
        }
        if (gdiplusToken != 0) {
            Gdiplus::Graphics graphics(dc);
            graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
            for (const UiSprite& sprite : uiSprites) {
                if (!sprite.visible || !uiGroupIsVisible(sprite.group)) continue;
                auto& image = desktopSpriteCache[sprite.path];
                if (!image) {
                    const std::wstring path = std::filesystem::u8path(sprite.path).wstring();
                    image = std::make_unique<Gdiplus::Image>(path.c_str());
                }
                if (image->GetLastStatus() != Gdiplus::Ok) continue;
                const Gdiplus::RectF destination(
                    static_cast<Gdiplus::REAL>(sprite.x * canvasWidth / 100.0),
                    static_cast<Gdiplus::REAL>(layout.headerHeight + sprite.y * canvasHeight / 100.0),
                    static_cast<Gdiplus::REAL>(sprite.width * canvasWidth / 100.0),
                    static_cast<Gdiplus::REAL>(sprite.height * canvasHeight / 100.0));
                graphics.DrawImage(image.get(), destination);
            }
        }
        for (const UiButton& button : uiButtons) {
            if (!button.visible || !uiGroupIsVisible(button.group)) continue;
            RECT area = {
                static_cast<LONG>(button.x * canvasWidth / 100.0),
                static_cast<LONG>(layout.headerHeight + button.y * canvasHeight / 100.0),
                static_cast<LONG>((button.x + button.width) * canvasWidth / 100.0),
                static_cast<LONG>(layout.headerHeight +
                    (button.y + button.height) * canvasHeight / 100.0)
            };
            unsigned red = 0, green = 0, blue = 0;
            parseColor(button.color, red, green, blue);
            const double opacity = std::clamp(button.opacity, 0.0, 1.0);
            const COLORREF blended = RGB(
                static_cast<BYTE>(red * opacity + 38 * (1.0 - opacity)),
                static_cast<BYTE>(green * opacity + 36 * (1.0 - opacity)),
                static_cast<BYTE>(blue * opacity + 44 * (1.0 - opacity)));
            HBRUSH buttonBrush = CreateSolidBrush(blended);
            if (button.cornerRadius > 0) {
                HPEN pen = CreatePen(PS_SOLID, 1, blended);
                HGDIOBJ oldPen = SelectObject(dc, pen);
                HGDIOBJ oldBrush = SelectObject(dc, buttonBrush);
                const int radius = std::max(1, static_cast<int>(button.cornerRadius * systemDpi() / 96.0));
                RoundRect(dc, area.left, area.top, area.right, area.bottom, radius * 2, radius * 2);
                SelectObject(dc, oldBrush); SelectObject(dc, oldPen); DeleteObject(pen);
            } else FillRect(dc, &area, buttonBrush);
            DeleteObject(buttonBrush);
            if (button.cornerRadius <= 0) FrameRect(dc, &area, static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));
            SetBkMode(dc, TRANSPARENT);
            SetTextColor(dc, RGB(255, 255, 255));
            const std::wstring label = wideFromUtf8(button.label);
            DrawTextW(dc, label.c_str(), -1, &area, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
        }
        for (const UiText& text : uiTexts) {
            if (!text.visible || !uiGroupIsVisible(text.group)) continue;
            unsigned red = 0, green = 0, blue = 0;
            if (!parseColor(text.color, red, green, blue)) continue;
            const int fontHeight = std::max(1, static_cast<int>(
                text.size * canvasHeight / 100.0));
            HFONT font = CreateFontW(-fontHeight, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
            HGDIOBJ previousFont = SelectObject(dc, font);
            SetBkMode(dc, TRANSPARENT);
            SetTextColor(dc, RGB(red, green, blue));
            const std::wstring value = wideFromUtf8(text.value);
            const LONG x = static_cast<LONG>(text.x * canvasWidth / 100.0);
            const LONG baseline = static_cast<LONG>(
                layout.headerHeight + text.y * canvasHeight / 100.0);
            RECT area = {x, baseline - fontHeight, client.right, baseline + fontHeight / 4};
            DrawTextW(dc, value.c_str(), -1, &area, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX);
            SelectObject(dc, previousFont);
            DeleteObject(font);
        }
        RECT outputArea = {
            layout.margin, layout.outputTop,
            std::max<LONG>(layout.margin, layout.width - layout.margin), layout.outputBottom
        };
        SetBkMode(dc, TRANSPARENT);
        SetTextColor(dc, RGB(245, 242, 255));
        DrawTextW(dc, outputBuffer.c_str(), static_cast<int>(outputBuffer.size()), &outputArea,
                  DT_LEFT | DT_TOP | DT_WORDBREAK | DT_NOPREFIX);
        if (bodyFont) {
            SelectObject(dc, previousBodyFont);
            DeleteObject(bodyFont);
        }
        EndPaint(window, &paint);
        return 0;
    }
    if (message == WM_CLOSE) {
        if (!confirmDiscardEditorChanges(window)) return 0;
        DestroyWindow(window);
        return 0;
    }
    if (message == WM_DESTROY) {
        mainWindow = nullptr;
        codeEditor = nullptr;
        openButton = saveButton = runButton = previewButton = newButton = nullptr;
        desktopSpriteCache.clear();
        if (gdiplusToken != 0) {
            Gdiplus::GdiplusShutdown(gdiplusToken);
            gdiplusToken = 0;
        }
        if (openButtonFont) {
            DeleteObject(openButtonFont);
            openButtonFont = nullptr;
        }
        if (codeEditorFont) {
            DeleteObject(codeEditorFont);
            codeEditorFont = nullptr;
        }
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(window, message, wParam, lParam);
}

LRESULT CALLBACK inputWindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_CREATE) {
        const auto scale = [window](int value) { return scaleForDpi(window, value); };
        CreateWindowExW(0, L"STATIC", inputPrompt.c_str(), WS_CHILD | WS_VISIBLE,
            scale(16), scale(16), scale(400), scale(36),
            window, nullptr, GetModuleHandleW(nullptr), nullptr);
        inputControl = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL,
            scale(16), scale(58), scale(400), scale(28),
            window, reinterpret_cast<HMENU>(inputEditId),
            GetModuleHandleW(nullptr), nullptr);
        CreateWindowExW(0, L"BUTTON", L"OK", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
            scale(330), scale(100), scale(86), scale(30),
            window, reinterpret_cast<HMENU>(inputButtonId),
            GetModuleHandleW(nullptr), nullptr);
        SetFocus(inputControl);
        return 0;
    }
    if (message == WM_COMMAND && LOWORD(wParam) == inputButtonId) {
        const int length = GetWindowTextLengthW(inputControl);
        std::wstring answer(static_cast<std::size_t>(length) + 1, L'\0');
        GetWindowTextW(inputControl, &answer[0], length + 1);
        answer.resize(static_cast<std::size_t>(length));
        inputResult = utf8FromWide(answer);
        inputAccepted = true;
        inputFinished = true;
        DestroyWindow(window);
        return 0;
    }
    if (message == WM_CLOSE) {
        inputAccepted = false;
        inputFinished = true;
        DestroyWindow(window);
        return 0;
    }
    return DefWindowProcW(window, message, wParam, lParam);
}

bool askForInput(const std::string& prompt, std::string& answer) {
#ifdef TRG_TEST_CONSOLE
    appendOutput(prompt + " ");
    return static_cast<bool>(std::getline(std::cin, answer));
#else
    inputPrompt = wideFromUtf8(prompt);
    inputFinished = false;
    inputAccepted = false;
    inputResult.clear();
    const wchar_t* className = L"TRGInputWindow";
    WNDCLASSW windowClass = {};
    windowClass.lpfnWndProc = inputWindowProc;
    windowClass.hInstance = GetModuleHandleW(nullptr);
    windowClass.lpszClassName = className;
    windowClass.hCursor = LoadCursorW(nullptr, MAKEINTRESOURCEW(32512));
    if (!RegisterClassW(&windowClass) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS) return false;
    if (mainWindow) EnableWindow(mainWindow, FALSE);
    const auto scale = [](int value) {
        return mainWindow ? scaleForDpi(mainWindow, value) : value;
    };
    HWND dialog = CreateWindowExW(WS_EX_DLGMODALFRAME, className, L"TRG input",
        WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, scale(450), scale(180),
        mainWindow, nullptr, windowClass.hInstance, nullptr);
    if (!dialog) {
        if (mainWindow) EnableWindow(mainWindow, TRUE);
        return false;
    }
    ShowWindow(dialog, SW_SHOW);
    UpdateWindow(dialog);
    MSG message;
    while (!inputFinished && GetMessageW(&message, nullptr, 0, 0) > 0) {
        if (!IsDialogMessageW(dialog, &message)) {
            TranslateMessage(&message);
            DispatchMessageW(&message);
        }
    }
    if (mainWindow) {
        EnableWindow(mainWindow, TRUE);
        SetForegroundWindow(mainWindow);
    }
    if (inputAccepted) answer = inputResult;
    return inputAccepted;
#endif
}

bool runUi() {
    enablePerMonitorDpiAwareness();
    Gdiplus::GdiplusStartupInput gdiplusInput;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusInput, nullptr);
    const wchar_t* className = L"TRGMainWindow";
    WNDCLASSW windowClass = {};
    windowClass.lpfnWndProc = mainWindowProc;
    windowClass.hInstance = GetModuleHandleW(nullptr);
    windowClass.lpszClassName = className;
    windowClass.hCursor = LoadCursorW(nullptr, MAKEINTRESOURCEW(32512));
    windowClass.hIcon = LoadIconW(windowClass.hInstance, MAKEINTRESOURCEW(IDI_TRG));
    if (!RegisterClassW(&windowClass) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS) return false;
    const int dpi = systemDpi();
    mainWindow = CreateWindowExW(0, className, L"TRG", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, MulDiv(1000, dpi, 96), MulDiv(720, dpi, 96),
        nullptr, nullptr, windowClass.hInstance, nullptr);
    if (!mainWindow) return false;
    uiStart = std::chrono::steady_clock::now();
    int fps = 60;
    for (const UiAnimation& animation : uiAnimations) fps = std::max(fps, animation.fps);
    if (!uiAnimations.empty() || !everyTasks.empty()) SetTimer(mainWindow, 1, static_cast<UINT>(std::max(1, 1000 / fps)), nullptr);
    ShowWindow(mainWindow, SW_SHOW);
    UpdateWindow(mainWindow);
    appendOutput("TRG is ready. Output and errors appear here.\r\n");
    return true;
}

void runMessageLoop() {
    MSG message;
    while (GetMessageW(&message, nullptr, 0, 0) > 0) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
}

#else
bool askForInput(const std::string& prompt, std::string& answer) {
#ifdef __ANDROID__
    bool accepted = false;
    answer = trgAskAndroid(prompt, accepted);
    return accepted;
#elif defined(TRG_WEB)
    (void)prompt; if (trgWebInputIndex >= trgWebInputLines.size()) return false; answer = trgWebInputLines[trgWebInputIndex++]; return true;
#else
    appendOutput(prompt + " "); return static_cast<bool>(std::getline(std::cin, answer));
#endif
}
#endif

std::vector<std::string> readProgram(std::istream& source) {
    std::vector<std::string> program;
    std::string line;
    bool firstLine = true;
    while (std::getline(source, line)) {
        if (firstLine && line.size() >= 3 &&
            static_cast<unsigned char>(line[0]) == 0xEF &&
            static_cast<unsigned char>(line[1]) == 0xBB &&
            static_cast<unsigned char>(line[2]) == 0xBF)
            line.erase(0, 3);
        firstLine = false;
        program.push_back(stripComment(line));
    }
    return program;
}

void executeProgram(const std::vector<std::string>& program) {
    uiStart = std::chrono::steady_clock::now();
#ifdef __ANDROID__
    androidStopRequested.store(false);
#endif
    uiVariables.clear();
    uiTypes.clear();
    uiDefaults = standardDefaults();
    keyboardControls.clear();
    controllerControls.clear();
    controlsEnabled = false;
    filesEnabled = false;
    jsonEnabled = false;
    webEnabled = false;
    collisionEnabled = false;
    collisionWatches.clear();
    webPageHtml.clear();
    Flow flow;
    errorCount = 0;
    uiBlocks.clear();
    uiSprites.clear();
    uiGroups.clear();
    currentUiGroup = "default";
#ifdef _WIN32
    desktopSpriteCache.clear();
#endif
    uiAnimations.clear();
    everyTasks.clear();
    uiButtons.clear();
    uiTexts.clear();
    userStructs.clear();
    variableGroups.clear();
    groupStack.clear();
    groupSnapshots.clear();
    executedFileKeys.clear();
    activeRunFiles.clear();

    
    
    
    uiProgram = mergePrograms(program, currentSourcePath, true, true);
    LibraryState libraries;
    scanLibraries(uiProgram, libraries);
    filesEnabled = libraries.files;
    jsonEnabled = libraries.json;
    webEnabled = libraries.web;
    collisionEnabled = libraries.collision;
    networkEnabled = libraries.network;
    if (libraries.controls) initializeControls(uiVariables, uiTypes);
    registerFunctions(uiProgram);
    executeBlock(uiProgram, 0, uiProgram.size(), uiVariables, uiTypes, uiDefaults, flow,
                 libraries.manual, libraries.ui);
    if (flow.stopped) everyTasks.clear();
    if (!everyTasks.empty()) appendOutput("Program started with repeating tasks.\r\n");
    else if (errorCount == 0) appendOutput("Program finished.\r\n");
    else appendOutput("Program finished with " + std::to_string(errorCount) + " error(s).\r\n");
#ifdef _WIN32
    if (mainWindow) {
        if (!uiAnimations.empty() || !everyTasks.empty()) SetTimer(mainWindow, 1, 16, nullptr);
        InvalidateRect(mainWindow, nullptr, TRUE);
    }
#elif !defined(__ANDROID__) && !defined(TRG_WEB)
    if (!uiBlocks.empty()) appendOutput("TRG UI is currently supported on Windows only.\n");
#endif
}

#ifdef _WIN32
bool loadSourceFile(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        appendOutput("ERROR Could not open \"" + path.u8string() + "\".\r\n");
        return false;
    }
    std::ostringstream contents;
    contents << file.rdbuf();
    std::string source = contents.str();
    if (source.size() >= 3 && static_cast<unsigned char>(source[0]) == 0xEF &&
        static_cast<unsigned char>(source[1]) == 0xBB && static_cast<unsigned char>(source[2]) == 0xBF)
        source.erase(0, 3);
    loadingEditorText = true;
    SetWindowTextW(codeEditor, wideFromUtf8(source).c_str());
    loadingEditorText = false;
    currentSourcePath = std::filesystem::absolute(path);
    projectRoot = currentSourcePath.parent_path();
    editorDirty = false;
    previewMode = false;
    if (previewButton) SetWindowTextW(previewButton, L"Preview");
    updateEditorWindowTitle();
    layoutOutputControls(mainWindow);
    outputBuffer.clear();
    appendOutput("Opened: " + currentSourcePath.u8string() + "\r\n");
    InvalidateRect(mainWindow, nullptr, TRUE);
    return true;
}

bool saveSourceFile() {
    std::filesystem::path path = currentSourcePath;
    if (!saveSourceDialog(path, mainWindow)) return false;
    const int length = GetWindowTextLengthW(codeEditor);
    std::wstring wideSource(static_cast<std::size_t>(length) + 1, L'\0');
    GetWindowTextW(codeEditor, wideSource.data(), length + 1);
    wideSource.resize(static_cast<std::size_t>(length));
    const std::string source = utf8FromWide(wideSource);
    std::ofstream output(path, std::ios::binary | std::ios::trunc);
    if (!output) {
        appendOutput("ERROR Could not save \"" + path.u8string() + "\".\r\n");
        return false;
    }
    output.write(source.data(), static_cast<std::streamsize>(source.size()));
    if (!output) {
        appendOutput("ERROR Failed while writing \"" + path.u8string() + "\".\r\n");
        return false;
    }
    currentSourcePath = std::filesystem::absolute(path);
    projectRoot = currentSourcePath.parent_path();
    editorDirty = false;
    updateEditorWindowTitle();
    appendOutput("Saved: " + currentSourcePath.u8string() + "\r\n");
    return true;
}

bool runEditorSource() {
    if (!codeEditor) return false;
    const int length = GetWindowTextLengthW(codeEditor);
    std::wstring wideSource(static_cast<std::size_t>(length) + 1, L'\0');
    GetWindowTextW(codeEditor, wideSource.data(), length + 1);
    wideSource.resize(static_cast<std::size_t>(length));
    const std::string source = utf8FromWide(wideSource);
    if (!currentSourcePath.empty()) projectRoot = currentSourcePath.parent_path();
    else projectRoot = std::filesystem::current_path();
    outputBuffer.clear();
    appendOutput("Running: " + (currentSourcePath.empty() ? std::string("Untitled.trg") : currentSourcePath.u8string()) + "\r\n\r\n");
    std::istringstream input(source);
    executeProgram(readProgram(input));
    InvalidateRect(mainWindow, nullptr, TRUE);
    return true;
}

bool runSourceFile(const std::filesystem::path& path) {
    return loadSourceFile(path) && runEditorSource();
}
#endif




#if defined(TRG_WEB)
extern "C" const char* trg_run_source(const char* source,const char* input){
 trgWebOutput.clear();trgWebInputLines.clear();trgWebInputIndex=0;
 if(input){std::istringstream in(input);std::string line;while(std::getline(in,line))trgWebInputLines.push_back(line);}
 currentSourcePath.clear();projectRoot=std::filesystem::current_path();
 std::istringstream in(source?source:"");executeProgram(readProgram(in));return trgWebOutput.c_str();
}
extern "C" const char* trg_get_output(){return trgWebOutput.c_str();}
extern "C" const char* trg_get_web_html(){return webPageHtml.c_str();}
extern "C" void trg_set_key(const char* key,int pressed){if(!key)return;if(pressed)trgWebPressedKeys.insert(key);else trgWebPressedKeys.erase(key);if(controlsEnabled)uiVariables[key]=pressed?"press":"released";}
extern "C" void trg_click_button(const char* name){if(!name)return;for(const auto& b:uiButtons)if(b.name==name&&b.visible&&uiGroupIsVisible(b.group)){Flow flow;executeBlock(b.body,0,b.body.size(),uiVariables,uiTypes,uiDefaults,flow,false,true);break;}}
extern "C" const char* trg_get_ui_state(){
 auto q=[](const std::string&s){std::string r="\"";for(unsigned char c:s){if(c=='\\'||c=='\"'){r+='\\';r+=char(c);}else if(c=='\n')r+="\\n";else if(c=='\r')r+="\\r";else if(c=='\t')r+="\\t";else if(c>=32)r+=char(c);}return r+"\"";};
 std::ostringstream o;o<<"{\"blocks\":[";
 for(size_t i=0;i<uiBlocks.size();++i){const auto&v=uiBlocks[i];if(i)o<<',';o<<"{\"name\":"<<q(v.name)<<",\"group\":"<<q(v.group)<<",\"color\":"<<q(v.color)<<",\"shape\":"<<q(v.shape)<<",\"x\":"<<v.x<<",\"y\":"<<v.y<<",\"width\":"<<v.width<<",\"height\":"<<v.height<<",\"opacity\":"<<v.opacity<<",\"visible\":"<<(v.visible?"true":"false")<<",\"z\":"<<v.z<<"}";}
 o<<"],\"texts\":[";
 for(size_t i=0;i<uiTexts.size();++i){const auto&v=uiTexts[i];if(i)o<<',';o<<"{\"name\":"<<q(v.name)<<",\"group\":"<<q(v.group)<<",\"value\":"<<q(v.value)<<",\"color\":"<<q(v.color)<<",\"font\":"<<q(v.font)<<",\"align\":"<<q(v.align)<<",\"x\":"<<v.x<<",\"y\":"<<v.y<<",\"size\":"<<v.size<<",\"opacity\":"<<v.opacity<<",\"bold\":"<<(v.bold?"true":"false")<<",\"visible\":"<<(v.visible?"true":"false")<<"}";}
 o<<"],\"buttons\":[";
 for(size_t i=0;i<uiButtons.size();++i){const auto&v=uiButtons[i];if(i)o<<',';o<<"{\"name\":"<<q(v.name)<<",\"group\":"<<q(v.group)<<",\"label\":"<<q(v.label)<<",\"x\":"<<v.x<<",\"y\":"<<v.y<<",\"width\":"<<v.width<<",\"height\":"<<v.height<<",\"color\":"<<q(v.color)<<",\"textColor\":"<<q(v.textColor)<<",\"opacity\":"<<v.opacity<<",\"cornerRadius\":"<<v.cornerRadius<<",\"visible\":"<<(v.visible?"true":"false")<<"}";}
 o<<"],\"sprites\":[";
 for(size_t i=0;i<uiSprites.size();++i){const auto&v=uiSprites[i];if(i)o<<',';o<<"{\"name\":"<<q(v.name)<<",\"group\":"<<q(v.group)<<",\"path\":"<<q(v.path)<<",\"x\":"<<v.x<<",\"y\":"<<v.y<<",\"width\":"<<v.width<<",\"height\":"<<v.height<<",\"opacity\":"<<v.opacity<<",\"visible\":"<<(v.visible?"true":"false")<<"}";}
 o<<"],\"groups\":[";
 for(size_t i=0;i<uiGroups.size();++i){const auto&v=uiGroups[i];if(i)o<<',';o<<"{\"name\":"<<q(v.name)<<",\"parent\":"<<q(v.parent)<<",\"visible\":"<<(v.visible?"true":"false")<<"}";}
 o<<"],\"animations\":[";
 for(size_t i=0;i<uiAnimations.size();++i){const auto&v=uiAnimations[i];if(i)o<<',';o<<"{\"name\":"<<q(v.name)<<",\"startX\":"<<v.startX<<",\"startY\":"<<v.startY<<",\"dx\":"<<v.dx<<",\"dy\":"<<v.dy<<",\"startTime\":"<<v.startTime<<",\"duration\":"<<v.duration<<",\"fps\":"<<v.fps<<"}";}
 o<<"],\"viewportX\":"<<viewportX<<",\"viewportY\":"<<viewportY<<"}";static std::string result;result=o.str();return result.c_str();
}
#else
#ifndef TRG_ANDROID_BRIDGE
int runApplication(int argc, char** argv) {
#if defined(_WIN32) && !defined(TRG_TEST_CONSOLE)
    if (!runUi()) {
        MessageBoxW(nullptr, L"Could not create the TRG window.", L"TRG error", MB_OK | MB_ICONERROR);
        return 1;
    }
#endif

    if (argc > 1 && lower(argv[1]) == "compile") {
        if (argc != 3) {
            appendOutput("ERROR Usage: TRG compile <file.trg>\r\n");
#if defined(_WIN32) && !defined(TRG_TEST_CONSOLE)
            runMessageLoop();
#endif
            return 2;
        }
        std::filesystem::path sourcePath = std::filesystem::u8path(argv[2]);
        if (!std::filesystem::exists(sourcePath) && sourcePath.extension().empty()) sourcePath += ".trg";
        std::ifstream source(sourcePath, std::ios::binary);
        if (!source) {
            appendOutput("ERROR Could not open \"" + sourcePath.u8string() + "\".\r\n");
#if defined(_WIN32) && !defined(TRG_TEST_CONSOLE)
            runMessageLoop();
#endif
            return 1;
        }
#ifdef _WIN32
        wchar_t executableBuffer[32768];
        const DWORD executableLength = GetModuleFileNameW(nullptr, executableBuffer, 32768);
        if (executableLength == 0 || executableLength >= 32768) {
            appendOutput("ERROR Could not locate the TRG executable.\r\n");
#if !defined(TRG_TEST_CONSOLE)
            runMessageLoop();
#endif
            return 1;
        }
        const std::filesystem::path executablePath(std::wstring(executableBuffer, executableLength));
#else
        const std::filesystem::path executablePath = std::filesystem::absolute(argv[0]);
#endif
        std::filesystem::path outputPath = sourcePath;
        outputPath.replace_extension(".exe");
        std::error_code error;
        const std::uint64_t payloadSize = std::filesystem::file_size(sourcePath, error);
        if (error) { appendOutput("ERROR Could not determine source size: " + error.message() + "\r\n"); return 1; }
        std::filesystem::copy_file(executablePath, outputPath, std::filesystem::copy_options::overwrite_existing, error);
        if (error) { appendOutput("ERROR Could not create " + outputPath.u8string() + ": " + error.message() + "\r\n"); return 1; }
        std::ofstream output(outputPath, std::ios::binary | std::ios::app);
        if (!output) { appendOutput("ERROR Could not write " + outputPath.u8string() + "\r\n"); return 1; }
        output << source.rdbuf();
        for (unsigned byte = 0; byte < 8; ++byte) output.put(static_cast<char>((payloadSize >> (byte * 8)) & 0xff));
        output.write("TRGPKG01", 8);
        output.close();
        if (!output) { appendOutput("ERROR Could not finish writing " + outputPath.u8string() + "\r\n"); return 1; }
        appendOutput("Created " + outputPath.u8string() + " (standalone TRG app).\r\n");
#if defined(_WIN32) && !defined(TRG_TEST_CONSOLE)
        runMessageLoop();
#endif
        return 0;
    }

    std::filesystem::path scriptPath;
    std::string embeddedSource;
    bool hasEmbeddedSource = false;
    if (argc > 1) {
        scriptPath = argv[1];
    } else {
#ifdef _WIN32
        wchar_t executableBuffer[32768];
        const DWORD executableLength = GetModuleFileNameW(nullptr, executableBuffer, 32768);
        if (executableLength > 0 && executableLength < 32768) scriptPath = std::filesystem::path(std::wstring(executableBuffer, executableLength));
#else
        scriptPath = std::filesystem::absolute(argv[0]);
#endif
        std::ifstream executable(scriptPath, std::ios::binary);
        if (executable) {
            executable.seekg(0, std::ios::end);
            const std::streamoff totalSize = executable.tellg();
            if (totalSize >= 16) {
                executable.seekg(totalSize - 8);
                char marker[8] = {};
                executable.read(marker, 8);
                if (executable && std::string(marker, 8) == "TRGPKG01") {
                    executable.seekg(totalSize - 16);
                    std::uint64_t payloadSize = 0;
                    for (unsigned byte = 0; byte < 8; ++byte) {
                        const int value = executable.get();
                        if (value == EOF) { payloadSize = 0; break; }
                        payloadSize |= static_cast<std::uint64_t>(static_cast<unsigned char>(value)) << (byte * 8);
                    }
                    if (payloadSize > static_cast<std::uint64_t>(totalSize - 16)) {
                        appendOutput("ERROR Invalid embedded TRG program.\r\n");
#if !defined(TRG_TEST_CONSOLE)
                        runMessageLoop();
#endif
                        return 1;
                    }
                    executable.seekg(totalSize - 16 - static_cast<std::streamoff>(payloadSize));
                    embeddedSource.resize(static_cast<std::size_t>(payloadSize));
                    if (payloadSize > 0) executable.read(&embeddedSource[0], static_cast<std::streamsize>(payloadSize));
                    if (!executable) {
                        appendOutput("ERROR Could not read embedded TRG program.\r\n");
#if !defined(TRG_TEST_CONSOLE)
                        runMessageLoop();
#endif
                        return 1;
                    }
                    hasEmbeddedSource = true;
                }
            }
        }
    }

    std::vector<std::string> program;
    if (hasEmbeddedSource) {
        std::istringstream input(embeddedSource);
        program = readProgram(input);
    } else if (!scriptPath.empty() && argc > 1) {
        std::ifstream file(scriptPath);
        if (!file.is_open()) {
            appendOutput("ERROR Could not open \"" + scriptPath.u8string() + "\".\r\n");
#if defined(_WIN32) && !defined(TRG_TEST_CONSOLE)
            runMessageLoop();
#endif
            return 1;
        }
#ifdef _WIN32
        SetWindowTextW(mainWindow, (L"TRG - " + scriptPath.filename().wstring()).c_str());
#endif
        program = readProgram(file);
        currentSourcePath = std::filesystem::absolute(scriptPath);
        projectRoot = currentSourcePath.parent_path();
#ifdef _WIN32
        loadSourceFile(scriptPath);
#endif
    } else if (argc == 1) {
#ifdef _WIN32
        if (mainWindow) runMessageLoop();
        return 0;
#else
        std::cout << "TRG Interpreter. Type 'exit' or 'stop' to quit.\n";
        std::string line;
        Vars variables;
        Types types;
        Defaults defaults = standardDefaults();
        Flow flow;
        while (std::getline(std::cin, line)) {
            line = stripComment(line);
            if (trim(line).empty()) continue;
            std::vector<std::string> interactiveProgram = {line};
            registerFunctions(interactiveProgram);
            executeBlock(interactiveProgram, 0, interactiveProgram.size(), variables, types, defaults, flow);
            if (flow.stopped) break;
        }
        return 0;
#endif
    }

    executeProgram(program);
#if defined(_WIN32) && !defined(TRG_TEST_CONSOLE)
    runMessageLoop();
#endif
    return 0;
}

// ---------------------------------------------------------

// Desktop application entry points
#if defined(_WIN32) && !defined(TRG_TEST_CONSOLE)
int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int) {
    int argumentCount = 0;
    LPWSTR* wideArguments = CommandLineToArgvW(GetCommandLineW(), &argumentCount);
    if (!wideArguments) {
        MessageBoxW(nullptr, L"Could not read the TRG command line.", L"TRG error", MB_OK | MB_ICONERROR);
        return 1;
    }
    std::vector<std::string> arguments;
    std::vector<char*> narrowArguments;
    arguments.reserve(static_cast<std::size_t>(argumentCount));
    narrowArguments.reserve(static_cast<std::size_t>(argumentCount));
    for (int i = 0; i < argumentCount; ++i) arguments.push_back(utf8FromWide(wideArguments[i]));
    LocalFree(wideArguments);
    for (std::string& argument : arguments) narrowArguments.push_back(&argument[0]);
    return runApplication(argumentCount, narrowArguments.data());
}
#endif
#if !defined(_WIN32) || defined(TRG_TEST_CONSOLE)
int main(int argc, char** argv) {
    return runApplication(argc, argv);
}
#endif

#endif // TRG_ANDROID_BRIDGE
#endif // TRG_WEB
