#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define MAX_IMG_FILE_LEN 256
#define MAX_CMD_LEN 256

static size_t      IMG_CNT        = 1;
static const char* IMG_FOLDER_DIR = "img/";

void EndGraph(FILE* dotf) {
    fprintf(dotf, "}");
}

void StartGraph(FILE* dotf) {
    fprintf(dotf, "digraph structs {\n"
	              "rankdir=TB;\n"
	              "node[color=\"black\",fontsize=14];\n"
                  "nodesep = 0.1;\n"
	              "edge[color=\"darkblue\",fontcolor=\"yellow\",fontsize=12];\n");
}

void MakeImgFromDot(const char* dot_file) {
    char img_name[MAX_IMG_FILE_LEN] = "";
    snprintf(img_name, MAX_IMG_FILE_LEN, "%simg%lu_%lu.png", IMG_FOLDER_DIR, IMG_CNT++, clock());

    char dot_command[MAX_CMD_LEN] = "";
    snprintf(dot_command, MAX_CMD_LEN, "dot %s -T png -o %s", dot_file, img_name);
    system(dot_command);

    printf("<img src=\"%s\"><br>", img_name);
}

void EndGraphic(FILE* gnuf) {
    fprintf(gnuf, "set out\n");
}

void StartGraphic(const char* gpl_file, const char* img_name) {
    assert(gpl_file);
    assert(img_name);

    FILE* gnuplot_file = fopen(gpl_file, "a");  // изменение "w" на "a"
    if (gnuplot_file != NULL) {
        fprintf(gnuplot_file, "#! /opt/homebrew/Cellar/gnuplot -persist\n"
                              "reset\n"
                              "set terminal png size 800, 600\n"
                              "set output '%s'\n"
                              "set xlabel \"X\"\n"
                              "set ylabel \"Y\"\n"
                              "set grid\n", img_name);
        fclose(gnuplot_file);

        char gnu_command[MAX_CMD_LEN] = "";
        snprintf(gnu_command, MAX_CMD_LEN, "gnuplot %s", gpl_file);
        system(gnu_command);

        printf("\n<img src=\"%s\"><br>\n", img_name);
    } else {
        printf("Error: Unable to open Gnuplot script file.\n");
    }
}


char* GenImgName() {
    char* img_name = (char*)calloc(MAX_IMG_FILE_LEN, sizeof(char));
    if (!img_name)
        return NULL;

    snprintf(img_name, MAX_IMG_FILE_LEN, "%simg%lu_%lu.png", IMG_FOLDER_DIR, IMG_CNT++, clock());

    return img_name;
}

void MakeImgFromGpl(const char* gpl_file, const char* img_name) {
    assert(gpl_file);
    assert(img_name);

    char gnu_command[MAX_CMD_LEN] = "";
    snprintf(gnu_command, MAX_CMD_LEN, "gnuplot %s", gpl_file);
    system(gnu_command);

    printf("\n<img src=\"%s\"><br>\n", img_name);
}

void CreateSinPlotScript(const char* script_file) {
    FILE* script = fopen(script_file, "a");
    assert(script != NULL);

    fprintf(script, "plot sin(x)\n");

    fclose(script);
}

int main() {
    // Создаем скрипт для графика синуса
    const char* gpl_file = "sin_plot.gpl";
    CreateSinPlotScript(gpl_file);

    // Генерируем уникальное имя изображения
    char* img_name = GenImgName();

    // Запускаем Gnuplot для создания изображения
    StartGraphic(gpl_file, img_name);

    // Создаем изображение из Gnuplot-скрипта
    MakeImgFromGpl(gpl_file, img_name);

    // Освобождаем выделенную память для имени изображения
    free(img_name);

    return 0;
}


