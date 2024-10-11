user_input = input("Введіть рядок: ")

words = user_input.split()

word_count = len(words)

longest_word = max(words, key=len)

print(f"Кількість слів у рядку: {word_count}")
print(f"Найдовше слово: {longest_word}")
