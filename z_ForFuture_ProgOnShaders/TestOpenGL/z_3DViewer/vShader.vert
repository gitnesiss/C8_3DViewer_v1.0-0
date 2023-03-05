attribute vec4 a_position;  // a - attribute, позиция
attribute vec2 a_texcoord;  // a - attribute, текстурные координаты
attribute vec3 a_normal;    // a - attribute, нормаль для каждой вершины
uniform mat4 u_projectionMatrix;// u - uniform, матрица проекции
uniform mat4 u_viewMatrix;      // u - uniform, видовая матрица (матрица камеры)
uniform mat4 u_modelMatrix;     // u - uniform, матрица проекции
varying vec4 v_position;    // для передачи текущей точки во фрагментный шейдер
varying vec2 v_texcoord;    // v - varyian,
varying vec3 v_normal;      // для передачи нормалей во фрагментный шейдер

void main(void)
{
     mat4 mv_matrix = u_viewMatrix * u_modelMatrix;  // модельно-видовая матрица

    gl_Position = u_projectionMatrix * mv_matrix * a_position;  // для формирования конечных координат вершин,
                // нужно умножить матрицу проекции на модельно-видовую матрицу и на координату текущую точку

    // далее нужно правильно передать все значения во фрагментный шейдер
    v_texcoord = a_texcoord;   // передаём текстурную координаты
    // во фрагментный шейдер нужно передать номаль не текущего объекта, а умноженную на модельно-видовую матрицу
    // mv_matrix размерности 4х4, а нормаль размерности 3х3, сответственно производим приведение этой матрицы
    v_normal = normalize(vec3(mv_matrix * vec4(a_normal, 0.0)));  // и окончательно приводим к вектору с тремя
                // компанентами и номируем его
    v_position = mv_matrix * a_position;  // умножаем на модельно-видовую матрицу для того чтобы смотреть на это всё в координатах камеры
}
