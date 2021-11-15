# Lab_1_Ros_Lyaptsev_2021-22
#### Первая лабораторная работа
Автор- Ляпцев Илья

____

# Запуск
0. На всякие случай напомню необходимые шаги при каждом запуске:
0.0 Закинуть папку с пакетом в /src workspace'са
0.1 Скомпилировать workspace со всеми пакетами
```
catkin_make
```
0.2 Из папки workspace'са запустить команду, чтобы он подтянул его к ros'овским источникам
```
source devel/setup.bash
```
0.3 Запустить ядро
```
roscore
```
1 Для запуска сервера необходимо написать, также подхватятся новые параметры
```
roslaunch ros_lab1 load_params.launch
```
2. Далее запускаем в казино игрока командой
```
rosrun ros_lab1 player <color> <number> <bet>
```
, где цвет- red, black, white

число- от 0 до 36

ставка- любое число, большее 0, изначально на счете 1000 монет

____

# В нашем казино обновление!
# Теперь ставить можно также только на цвет, чет\нечет и больше/меньше!
```
rosrun ros_lab1 player <bet> <how_much>
```
, где ставка- это black/red/green, odd/even, passe/manque
сколько- аналогично денежная сумма

____

# Наслаждайтесь игрой!
