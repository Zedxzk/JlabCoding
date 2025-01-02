import openpyxl # type: ignore


workbook = openpyxl.load_workbook("template.xlsx")
worksheet = workbook["Sheet1"]
for i in range(5000):
    for j in range(10):
        worksheet.cell(i + 1, j + 1, "")

workbook.save("template.xlsx")