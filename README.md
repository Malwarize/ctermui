<div align="center">

<img src="https://github.com/Malwarize/ctermui/assets/130087473/ca6b76b4-a2ec-4c2b-8c50-ba7c6fad8eca" alt="CTERMUI Logo" width="400">

<h2>🖥️ Build Modern Terminal UIs in C</h2>

[![GitHub release](https://img.shields.io/github/v/release/Malwarize/ctermui?color=blue&label=release)]()
[![GitHub license](https://img.shields.io/github/license/Malwarize/ctermui?color=green)]()
[![GitHub issues](https://img.shields.io/github/issues/Malwarize/ctermui?color=red)]()
[![GitHub stars](https://img.shields.io/github/stars/Malwarize/ctermui?color=yellow)]()
[![GitHub forks](https://img.shields.io/github/forks/Malwarize/ctermui?color=orange)]()
[![GitHub watchers](https://img.shields.io/github/watchers/Malwarize/ctermui?color=blue)]()

✨ **Create interactive, component-based terminal applications with ease.**

</div>

---

## 🗺️ Table of Contents
- [`📦 Installation`](#-installation)
- [`⚡ Features`](#-features)
- [`🚀 Usage`](#-usage)
- [`📌 Examples`](#-examples)
- [`🤝 Contributing`](#-contributing)

---

## ⚡ Features

- 🧩 **Component-Based UI**: Buttons, labels, progress bars, bar charts, scatter plots, forms, and more
- 📐 **Flexible Layouts**: Horizontal/vertical, nested, and percentage-based
- 🎨 **Custom Components**: Easily extend with your own draw logic
- ⌨️ **Keyboard Event Handling**: Register callbacks for interactive UIs
- 🛠️ **Simple API**: Clean, modular, and easy to use

---

## 📦 Installation

Clone the repository and build with `make`:
```sh
git clone https://github.com/Malwarize/ctermui.git
cd ctermui
make
```

---

## 🚀 Usage

### Minimal Example
```c
#include <ctermui_screen.h>
#include <ctermui_layout.h>
#include <ctermui_component.h>

int main() {
    ctermui_screen_t screen = ctermui_screen_init();
    ctermui_layout_t root = ctermui_layout_new_root(
        CTERMUI_HORIZONTAL, screen->width, screen->height
    );
    ctermui_component_t label = ctermui_new_text(
        "hello_label", "Hello, CTERMUI!", CTERMUI_BRIGHT_CYAN, CTERMUI_EMPTY, CTERMUI_ALIGN_CENTER
    );
    ctermui_layout_add_component(root, label);
    ctermui_screen_set_layout_root(screen, root);
    ctermui_screen_loop_start(screen, NULL, 10000);
    return 0;
}
```

---

## 📌 Examples

- [Button navigation](examples/button_navigation.c)
- [System usage](examples/grabage_linux_sys_usage.c)
- [Star Wars](examples/star_wars.c)
- [Scatter plot](examples/plot_sin.c)
- [Progress bar](examples/progress_bar.c)
- [Bar chart](examples/two_barcharts.c)

Explore more in the [`examples/`](examples/) directory.

---

## 🤝 Contributing

💡 Pull requests are welcome! Open an issue before major changes.

---

