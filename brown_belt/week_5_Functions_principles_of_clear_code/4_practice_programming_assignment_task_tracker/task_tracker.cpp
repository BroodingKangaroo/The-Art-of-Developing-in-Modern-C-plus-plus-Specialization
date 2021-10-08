#include <map>
#include <unordered_map>
#include <string>

#include <iostream>


using namespace std;

//enum class TaskStatus {
//    NEW,
//    IN_PROGRESS,
//    TESTING,
//    DONE
//};

//using TasksInfo = map<TaskStatus, int>;

class TeamTasks {
    map<string, TasksInfo> name_to_task_info;
public:
    const TasksInfo &GetPersonTasksInfo(const string &person) const {
        return name_to_task_info.at(person);
    }

    void AddNewTask(const string &person) {
        if (name_to_task_info.count(person)) {
            name_to_task_info[person][TaskStatus::NEW]++;
        } else {
            name_to_task_info[person] = {{TaskStatus::NEW, 1}};
        }
    }

    tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string &person, int task_count) {
        if (!name_to_task_info.count(person)) {
            return make_tuple<TasksInfo, TasksInfo>({}, {});
        }

        TasksInfo tasks_to_update;
        extract_tasks_to_update(name_to_task_info[person], tasks_to_update, task_count, TaskStatus::NEW);
        extract_tasks_to_update(name_to_task_info[person], tasks_to_update, task_count, TaskStatus::IN_PROGRESS);
        extract_tasks_to_update(name_to_task_info[person], tasks_to_update, task_count, TaskStatus::TESTING);

        TasksInfo untouched_tasks = name_to_task_info[person];

        TasksInfo updated_tasks = update_tasks(tasks_to_update, name_to_task_info[person]);

        if (untouched_tasks.count(TaskStatus::DONE)) {
            untouched_tasks.erase(TaskStatus::DONE);
        }

        return make_tuple(updated_tasks, untouched_tasks);
    }

private:
    void extract_tasks_to_update(TasksInfo &person_tasks, TasksInfo &tasks_to_update, int &task_count,
                                 TaskStatus task_status) {
        if (task_count && person_tasks.count(task_status)) {
            if (task_count >= person_tasks[task_status]) {
                tasks_to_update[task_status] = person_tasks[task_status];
                task_count -= person_tasks[task_status];
                person_tasks[task_status] = 0;
                person_tasks.erase(task_status);
            } else {
                tasks_to_update[task_status] = task_count;
                person_tasks[task_status] -= task_count;
                task_count = 0;
            }
        }
    }

    TasksInfo update_tasks(TasksInfo &tasks_to_update, TasksInfo &person_tasks) {
        TasksInfo updated_tasks;
        for (const auto &[status, number] : tasks_to_update) {
            auto next_task_status = static_cast<TaskStatus>(static_cast<int>(status) + 1);
            updated_tasks[next_task_status] = number;
            if (person_tasks.count(next_task_status)) {
                person_tasks[next_task_status] += number;
            } else {
                person_tasks[next_task_status] = number;
            }
        }
        return updated_tasks;
    }
};

void PrintTasksInfo(TasksInfo tasks_info) {
    cout << ((tasks_info.count(TaskStatus::NEW) !=0 ) ? to_string(tasks_info[TaskStatus::NEW]) : "*")  <<
         ", " << ((tasks_info.count(TaskStatus::IN_PROGRESS) !=0 ) ? to_string(tasks_info[TaskStatus::IN_PROGRESS]) : "*") <<
         ", " << ((tasks_info.count(TaskStatus::TESTING) !=0 ) ? to_string(tasks_info[TaskStatus::TESTING]) : "*") <<
         ", " << ((tasks_info.count(TaskStatus::DONE) !=0 ) ? to_string(tasks_info[TaskStatus::DONE]) : "*") << endl;
}

int main() {
    TeamTasks tasks;
    for (int i = 0; i < 5; ++i) {
        tasks.AddNewTask("Ivan");
    }

    TasksInfo updated_tasks, untouched_tasks;

    tie(updated_tasks, untouched_tasks) =
            tasks.PerformPersonTasks("Ivan", 10);
    PrintTasksInfo(updated_tasks);
    PrintTasksInfo(untouched_tasks);

    cout << endl;

    tie(updated_tasks, untouched_tasks) =
            tasks.PerformPersonTasks("Ivan", 10);
    PrintTasksInfo(updated_tasks);
    PrintTasksInfo(untouched_tasks);


    cout << endl;


    for (int i = 0; i < 5; ++i) {
        tasks.AddNewTask("Ivan");
    }

    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));

    cout << endl;

    tie(updated_tasks, untouched_tasks) =
            tasks.PerformPersonTasks("Ivani", 1);
    PrintTasksInfo(updated_tasks);
    PrintTasksInfo(untouched_tasks);

    return 0;
}
