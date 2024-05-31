// ToDo.cpp: определяет точку входа для приложения.
//

#include "ToDo.h"
#include <iostream>
#include <algorithm>
#include <ctime>
#include <vector>
#include <string>
#include <iomanip>

// Создадим структуру, которая используется для хранения
// информации о текущем задании.
struct task
{
	std::string name;
	double duration;
	bool status;
};

bool checkTaskName(std::vector<task>& tasks, std::string& taskName)
{	// Функция проверяет, если такое имя задания в векторе заданий
	// Если имя уже используется, то возвращается FALSE
	for (int i = 0; i < tasks.size(); ++i)
	{
		if (tasks[i].name == taskName)
			return false;
	}
	return true;
}

void finishTask(std::vector<task>& tasks, task& purpose, std::time_t& tBegin)
{	// Если текущее задание существует (его имя не ""), то
	// фунция завершает текущее задание и записывает его в вектор заданий,
	// обнуляет текущее задание
	// Если текущего задания нет, то ничего не делает
	// Функция ничего не возвращает.
	if (purpose.name != "")
	{
		purpose.duration = difftime(std::time(nullptr), tBegin);
		purpose.status = false;
		tasks.push_back(purpose);
		std::cout << "The task " << purpose.name << " was finished.\n";
		purpose.name = "";
	}
}

void newTask(std::vector<task>& tasks, task& purpose, std::time_t& tBegin)
{	// Функция создания нового задания
	// Если введённое имя задания раньше не встречалось, то текущее задание завершается
	// И создаётся новое задание
	// Если имя задания уже использовалось ранее, то выводится соответствующее сообщение.
	std::tm* t_begin;
	std::string taskName;
	
	tBegin = std::time(nullptr);
	t_begin = std::localtime(&tBegin);
	std::cout << "Enter name task: ";
	std::cin >> taskName;
	if(purpose.name == taskName || !checkTaskName(tasks, taskName))
		std::cout << "The task " << taskName << " already create.\n";
	else
	{
		if(purpose.name != "")
			finishTask(tasks, purpose, tBegin);
		purpose.name = taskName;
		purpose.status = true;
		std::cout << "The task " << purpose.name << " started at: " << std::put_time(t_begin, "%d %B %Y %H:%M:%S") << '\n';
	}
}

void showStatus(std::vector<task>& tasks, task& purpose, std::time_t& tBegin)
{	// Выводит на экран список завершённых заданий и текущее задание.
	// статус задания выводится не FALSE или TRUE, а active или finished
	// Функция ничего не возвращает
	std::string status, outStr;
	int days, hours, minutes, seconds;
	std::tm* t_begin;
	t_begin = std::localtime(&tBegin);
	for (int i = 0; i < tasks.size(); ++i)
	{
		status = tasks[i].status ? "active" : "finished";
		// Перевод секунд в сутки / часы / минуты / секунды
		days = (int)tasks[i].duration / 24 / 60 / 60;
		hours = (int)tasks[i].duration % (24 * 60 * 60) / 3600;
		minutes = (int)tasks[i].duration % (24 * 60 * 60) % 3600 / 60;
		seconds = (int)tasks[i].duration % (24 * 60 * 60) % 3600 % 60 % 60;
		outStr = "Name of task: " + tasks[i].name + " duration: ";
		if (days > 0)
			outStr += std::to_string(days) + " days ";
		if (hours > 0)
			outStr += std::to_string(hours) + " hours ";
		if (minutes > 0)
			outStr += std::to_string(minutes) + " minutes ";
		if (seconds > 0)
			outStr += std::to_string(seconds) + " seconds ";
		std::cout << outStr << "status: " << status << '\n';
	}
	if (purpose.name != "")
		std::cout << "Name of current task: " << purpose.name << " started at " << std::put_time(t_begin, "%d %B %Y %H:%M:%S") << " active.\n";
	else if (tasks.size() == 0)
		std::cout << "List tasks is empty.\n";
}

int main()
{	// Главная функция, в которой обрабатываются команды с клавиатуры
	// и вызываются необходимые процедуры.
	std::vector<task> tasks;
	task purpose = {};
	std::string cmd;
	bool finish = false;
	std::time_t tBegin;
	
	while(!finish)
	{
		std::cout << "Enter command (BEGIN, END, STATUS or BYE): ";
		std::cin >> cmd;
		std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
		
		if (cmd == "BEGIN")
			newTask(tasks, purpose, tBegin);
		else if (cmd == "END")
			finishTask(tasks, purpose, tBegin);
		else if (cmd == "STATUS")
			showStatus(tasks, purpose, tBegin);
		else if (cmd == "BYE")
			finish = true;
		else
			std::cout << "Unknown command.\n";
	}
	std::cout << "Thank you for use my program.\n";
	return 0;
}
