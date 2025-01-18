# Contributing to Arduino Weather Station

Thank you for considering contributing to the Arduino Weather Station Project! We welcome contributions from the community to improve this project. Please follow the guidelines below to ensure a smooth collaboration process.

## How to Contribute

### 1. Fork the Repository
- Fork this repository to your GitHub account.
- Clone your forked repository to your local machine:
  ```bash
  git clone git@github.com:<your-username>/arduino_smart_weather_station.git
  cd arduino_smart_weather_station
  ```

### 2. Set Upstream Remote
- Add the upstream repository to keep your fork up-to-date:
  ```bash
  git remote add upstream git@github.com:kibshh/arduino_smart_weather_station.git
  ```

### 3. Create a Branch
- Always create a new branch for your work:
  ```bash
  git checkout -b feature-or-bugfix-name
  ```

### 4. Make Your Changes
- Write clean, readable, and well-documented code.
- Ensure your code follows the project’s coding standards.

### 5. Test Your Changes
- Thoroughly test your changes to confirm they work as intended.
- If you don't own an Arduino or the necessary sensors/parts, please add a comment to your related Trello card or in the PR so someone else can test your changes.

### 6. Commit Your Changes
- Use [Semantic Commit Messages](https://www.conventionalcommits.org/) for your commits. Examples:
  - `feat(sensor): add support for DHT11 temperature sensor`
  - `fix(display): correct issue with incorrect humidity readings`
  - `feat: task_cyclicTask implementation for RTC`

### 7. Push Your Branch
- Push your branch to your forked repository:
  ```bash
  git push origin feature-or-bugfix-name
  ```

### 8. Submit a Pull Request
- Open a pull request (PR) from your branch to the `main` branch of the original repository.
- In the name of the PR, add your Trello card ID. With examples from before:
  - `0AGHMOJ4 - feat(sensor): add support for DHT11 temperature sensor`
  - `9IBVFYT6 - fix(display): correct issue with incorrect humidity readings`
  - `456qosgz - feat: task_cyclicTask implementation for RTC`
- The ID at the beginning is your Trello card ID. To obtain it, open your card on Trello and copy the ID that comes after `/c/` e.g. `https://trello.com/c/<your-trello-card-id>/<card-number>-<card-name>`

- Provide a clear and concise description of your changes in the PR.

## Feedback and Questions
To get access to the Trello board, which is required to be able to contirbute, please reach out to either [kibshh](https://github.com/kibshh) or [markocupic024](https://github.com/markocupic024)

If you have questions or need help, feel free to open a [GitHub issue](https://github.com/kibshh/arduino_smart_weather_station/issues) or start a discussion.

We appreciate your contributions and thank you.
