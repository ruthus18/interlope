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
void input_init();  // set up kb and mouse

/* check kb and mouse, update runtime vars (on every frame) */
//
void input_update() {
    ...
}
```


### Module interface

* use namespace prefix for callable: `{namespace}_{function}`

**Example:**

`render.h`:
```c
void render_init();

uint32_t render_load_shader(const char* path, int shader_type);
uint32_t render_get_uniform_var(const char* var_name);

bool render_check_stop();
bool render_check_error();
```
