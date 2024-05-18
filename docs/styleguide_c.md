## C Language Styleguide Convensions (related to engine)


### Comments

* use `// comment` for line comments
* use `/* comment */` for docstrings and block comments

**Example:**
```c

/* =====  OpenGL Vars  ====== */
uint32_t vao[NUM_VAO];  // Vertex Array Objects
uint32_t vbo[NUM_VBO];  // Vertex Buffer Objects


/* =====  Main Loop  ===== */
void init_input();  // set up kb and mouse

/* check kb and mouse, update runtime vars (on every frame) */
//
void update_input() {
    ...
}
```
