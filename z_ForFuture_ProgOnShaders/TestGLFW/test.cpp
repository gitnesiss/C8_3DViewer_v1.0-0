// https://habr.com/ru/post/311808/
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>  // подключает gl.h. GLEW управляет указателями на функции OpenGL
#include <GLFW/glfw3.h>

// Прототип функции
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Размеры окна
const GLuint WIDTH = 800, HEIGHT = 600;

// Шейдеры
const GLchar* vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "void main() {\n"
    "   gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
    "}\0";
const GLchar* fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 color;\n"
    "void main() {\n"
    "   color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

int main() {
    //Инициализация GLFW
    if (!glfwInit()) {
        std::cout << "Не инициализировалась библиотека GLFW." << std::endl;
        return -1;
        // _Exit(EXIT_SUCCESS); // для с
    }

    //Настройка GLFW
	//Задается минимальная требуемая версия OpenGL. 
	//Мажорная 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    //Минорная
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    //Установка профайла для которого создается контекст
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Выключение возможности изменения размера окна
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Создание объекта окна и его инициализация
    // Выстаавляем размер окна, но при этом OpenGL не знает какой размер окна.
    // Сделать известным размер окна для OpenGL можно с помощью функции glViewport
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Установка функций обратного вызова
    glfwSetKeyCallback(window, key_callback);

    // Инициализация GLEW
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    // Установка значение в true, чтобы GLEW знал, как использовать современный
    // подход к получению указателей функций и расширений.
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Установка значений в функцию glViewport
    // Установка размеров области просмотра
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    // Первые два аргумента функции это позиция нижнего левого угла
    // Третий и четвёртый это ширина и высота отрисовываемого окна в пикселях (px),
    // которые получаются напрямую из GLFW
    glViewport(0, 0, width, height);



    // Сборка и компиляция программы шейдера
    // Вершинный шейдер
    // Создание объекта вершинного шейдера
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Привязка исходного кода шейдера к объекту вершинного шейдера
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // Компиляция объекта вершинного шейдера
    glCompileShader(vertexShader);
    // Проверка ошибки по времени компиляции
    // Объявление переменной для хранения результата состояния
    GLint success;
    // Создание контейнера для хранения результата проверки
    GLchar infoLog[512];
    // Проверка успешности сборки вершинного шейдера
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    // Если проверка не прошла, то вывести ошибку
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Сборка и компиляция программы шейдера
    // Фрагментный шейдер
    // Создание объекта фрагментного шейдера
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Привязка исходного кода фрагментного шейдера к объекту фрагментного шейдера
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // Компиляция объекта фрагментного шейдера
    glCompileShader(fragmentShader);
    // Проверка ошибки по времени компиляции
    // Проверка успешности сборки фрагментного шейдера
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    // Если проверка не прошла, то вывести ошибку
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Связать вершинные и фрагментные шейдеры
    // Создание шейдерной программы
    GLuint shaderProgram = glCreateProgram();
    // Присоединение шейдеров
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // Связка шейдерной программы
    glLinkProgram(shaderProgram);
    // Проверка ошибки при привязке
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    // Если проверка не прошла, то вывести ошибку
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    // Удаление созданных шейдеров после связывания
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);



    // Настройка данных вершин (и буферов) и атрибутов указателей
    // GLfloat vertices[] = {  // Вершины для треугольника
    //     -0.5f, -0.5f, 0.0f, // Левый
    //      0.5f, -0.5f, 0.0f, // Правый
    //      0.0f,  0.5f, 0.0f  // Верхний
    // };
    GLfloat vertices[] = {  // Вершины для двух треугольников
         0.5f,  0.5f, 0.0f,  // Верхняя правая
         0.5f, -0.5f, 0.0f,  // Нижняя правая
        -0.5f, -0.5f, 0.0f,  // Нижняя левая
        -0.5f,  0.5f, 0.0f   // Верхняя левая
    };
    GLuint indices[] = {  // Массив вершин
        0, 1, 3,  // Первый треугольник
        1, 2, 3   // Второй треугольник
    };
    // Объявление переменных для объектов
    GLuint VBO, VAO, EBO;
    // Генерация объекта вершинного массива VAO - vertex array object
    glGenVertexArrays(1, &VAO);
    // Генерация объекта вершинного буфера VBO - vertex buffer object
    glGenBuffers(1, &VBO);
    // Генерация объекта индексного буфера EBO - element buffer object
    glGenBuffers(1, &EBO);
    // Связывание VAO
    glBindVertexArray(VAO);
    

    // Связывание VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Копируем массив вершин в буфер
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Связывание EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // Копируем массив индексов в буфер
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Сообщаем OpenGL как он должен интерпритировать вершинные данные
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    // Включаем атрибут, передаём вершинному атрибуту позиции аргумента
    glEnableVertexAttribArray(0);

    // Отмена привязки
    // Обратить внимание, вызов glVertexAttribPointer выполняет привязку 
    // VBO, однако в данный момент такой записью можно отвязать.
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    
    // Отключить VAO (всегда необходимо отвязывать любой буфер/массив,
    // чтобы предотвратить странные ошибки)
    glBindVertexArray(0); 



    // // Метод отрисовки примитивов - проволочный вид.
    // // арг.1 - отрисовка передней и задней части; арг.2 - отрисовывать только линии
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // арг.2 - отрисовывать закрашенную фигуру


    // Включаем зацикленную отрисовку для того чтобы окно не закрывалось,
    // пока его не закроют
    // Функция glfwWindowShouldClose проверяет в начале каждой итерации цикла,
    // получил ли GLFW инструкцию к закрытию
    while(!glfwWindowShouldClose(window)) {
        // Проверяет были ли вызваны какие либо события (вроде ввода с клавиатуры
        // или перемещение мыши) и вызывает установленные функции
        glfwPollEvents();

        // Команды отрисовки Render
        // Установка цвета которым будет залито окно во время очистки буфера.
        // Это функция устанавливает состояние
        glClearColor(0.2f, 0.3f, 0.3f, 1.0);
        // Очистка буфера. Эта функция использует состояние
        glClear(GL_COLOR_BUFFER_BIT);

        // Отрисовка фигуры
        // 
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Заменяет цветовой буфер (большой буфер, содержащий значения цвета 
        // для каждого пикселя в GLFW окне), который использовался для отрисовки 
        // во время текущей итерации и показывает результат на экране.
        glfwSwapBuffers(window);
    }
    // Очистка выделенных ресурсов.
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // Завершить GLFW, очистив все ресурсы, выделенные GLFW.
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // Когда пользователь нажимает ESC, мы устанавливаем свойство WindowShouldClose в true, 
    // и приложение после этого закроется
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    	glfwSetWindowShouldClose(window, GL_TRUE);
}
