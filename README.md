# 🖼️ ToPDF

![Language](https://img.shields.io/badge/Language-C%2B%2B20-blue.svg?style=flat-square)
![GUI](https://img.shields.io/badge/GUI-Qt%20Widgets-brightgreen.svg?style=flat-square)
![Platform](https://img.shields.io/badge/Platform-Windows-0078d7.svg?style=flat-square)
![Output](https://img.shields.io/badge/Output-PDF%20(QPdfWriter)-red.svg?style=flat-square)
![License](https://img.shields.io/badge/License-MIT-green.svg?style=flat-square)

A lightweight **Qt Widgets desktop app** that turns a batch of photos into a single, print-ready PDF — no online converters, no uploading personal photos to a random website.

Drop in your images, reorder and rotate them right on the thumbnail grid, pick a save location, and export. Each page is generated at print resolution from the original full-size image, not from a downscaled preview.

---

## ✨ Key Features

### 1. Flexible Photo Input
* **Native file dialog** — select multiple images at once (`.png`, `.jpg`, `.jpeg`, `.jfif`).
* **Drag & drop from the desktop** — drop image files straight onto the window.

### 2. Interactive Thumbnail Grid
* Photos are laid out in a scrollable `QGridLayout`, each as a self-contained `PhotoThumbnail` widget.
* **Per-photo rotate button**, overlaid directly on the thumbnail — rotates in 90° steps, re-rendered from the original file each time to avoid repeated-resample quality loss.
* **Per-photo remove button** — deletes a single photo and automatically reflows the remaining grid to close the gap.
* **Internal drag & drop reordering** — drag one thumbnail onto another to swap their positions, implemented via a custom `QMimeData` format carrying the source widget's identity.

### 3. High-Quality PDF Export
* Powered by `QPdfWriter` + `QPainter`, rendering at **300 DPI** on A4 pages.
* Each photo is loaded fresh from its original file at export time (not from the on-screen thumbnail), then scaled to fit the page **with aspect ratio preserved** and centered — no stretching, no cropping.
* Applied rotations are baked into the export automatically.
* Automatic pagination: a new page is started only when the next photo actually needs one, avoiding blank trailing pages.

### 4. Clean, Focused UI
* Auto-scrolling, distraction-free layout: just a photo grid, a save-path picker, and an export button.
* Custom multi-resolution application icon (window + taskbar + executable).
* Packaged as a standalone Windows installer (Inno Setup) with all Qt runtime dependencies bundled via `windeployqt` — runs on machines without Qt installed.

---

## 🛠️ Tech Stack

* **Language:** C++20
* **GUI:** Qt 6 Widgets (`QMainWindow`, `QGridLayout`, `QScrollArea`, custom `QLabel` subclass)
* **PDF Generation:** `QPdfWriter`, `QPainter`
* **Drag & Drop:** `QDrag`, `QMimeData`, custom MIME type for internal reordering
* **Packaging:** Inno Setup, `windeployqt`

---

## 🚀 How to Build & Run

### Prerequisites
* C++22 compatible compiler
* Qt 6.11.2 (Qt Widgets module) with Qt VS Tools

### Building from Source
```bash
git clone https://github.com/usevel/ToPDF.git
cd ToPDF
```
Open the project in Visual Studio with the **Qt VS Tools** extension installed, select the **Release** configuration, and build the `ToPdf` target.

---

## 🎯 Usage
1. Launch the app and add photos — either via **"Choose file"** or by dragging image files onto the window.
2. Rotate or remove individual photos using the buttons overlaid on each thumbnail.
3. Drag a thumbnail onto another to swap their order in the final PDF.
4. Click **"Browse..."** to choose where the finished PDF should be saved.
5. Click **"Convert to PDF"** — each photo is placed on its own page, scaled to fit and centered.

---

## 📜 License
Distributed under the **MIT License**. Feel free to inspect, modify, and use this code for learning and personal projects.