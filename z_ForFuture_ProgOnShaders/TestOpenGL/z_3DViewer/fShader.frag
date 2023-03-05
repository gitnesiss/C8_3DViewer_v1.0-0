uniform sampler2D u_texture;
uniform vec4 u_lightPosition;   // позиция точки освещения
uniform float u_lightPower; // сила источника света
varying vec4 v_position;    // для передачи текущей точки во фрагментный шейдер
varying vec2 v_texcoord;    // v - varyian,
varying vec3 v_normal;      // для передачи нормалей во фрагментный шейдер

void main(void)
{
    // объявляем результирующий цвет
    vec4 resultColor = vec4(0.0, 0.0, 0.0, 0.0);
//    vec4 resultColor = vec4(0.51, 0.83, 0.98, 1.0);
    //
    vec4 eyePosition = vec4(0.0, 0.0, 0.0, 1.0);  // это не вектор это именно позиция
    // сохраняем диффъюзный цвет материала
    vec4 diffMatColor = texture2D(u_texture, v_texcoord);  // текстура,              !!!!! чтобы работали текстуры
//    vec4 diffMatColor = resultColor;
    // объявление вектора взгляда
    // из v_position возьмём только три вектора - вектор где находится наблюдатель
    vec3 eyeVect = normalize(v_position.xyz - eyePosition.xyz);
    // объявляем вектор света
    vec3 lightVect = normalize(v_position.xyz - u_lightPosition.xyz);  // !!! если не работает (v_position.xyz - u_lightPosition)
    // объявляем вектор отражения света - куда отразится свет (функция reflect)
    // на вход reflect принимает вектор направления света и нормаль относительно которой надо отразить свет
    vec3 reflectLight = normalize(reflect(lightVect, v_normal));  //                 !!!!! чтобы верно работали блики и отражения света
//    vec3 reflectLight = normalize(reflect(lightVect, resultColor.xyz));
    // расчёт длины от наблюдателя до рассматриваемой точки
    float len = length(v_position.xyz - eyePosition.xyz);
    // следующая переменная отвечает за то на сколько большим будет пятно отражения на модели
    float specularFactor = 100.0;
    // следующая переменная отвечает за то на сколько сильно будет светиться сам материал
    float ambientFactor = 0.1;

    // Далее производим сам расчёт
    // Диффьюзный цвет рассчитывается - уменьшение отражения света при удаление от источника
    // max(0.0, ...), максимум между нулём и скалярным произведением (dot)    - отсекаем отрицательные значения
    vec4 diffColor = diffMatColor * u_lightPower * max(0.0, dot(v_normal, -lightVect)) /
            (1.0 + 0.15 * pow(len, 2.0));  // (1.0 + 0.15 * pow(len, 2.0));
//    vec4 diffColor = diffMatColor * u_lightPower * max(0.0, dot(resultColor.xyz, -lightVect)) / (1.0 + 0.15 * pow(len, 2.0));
    resultColor += diffColor;

    // вычисляем амбиент - лёгкое отражение от модели при максимальной темноте (делает немного светлее)
    vec4 ambientColor = ambientFactor * diffMatColor;
    resultColor += ambientColor;

    // добавляем блики на модели от источников освещения
    vec4 specularColor = vec4(1.0, 1.0, 1.0, 1.0) * u_lightPower *
            pow(max(0.0, dot(reflectLight, -eyeVect)), specularFactor) / (1.0 + 0.15 * pow(len, 2.0));
    resultColor += specularColor;

    gl_FragColor = resultColor;

//    gl_FragColor = vec4(0.51, 0.83, 0.98, 1.0);  // фигура одним цветом
//    gl_FragColor = texture2D(u_texture, v_texcoord);  // текстура
}
