// Sunset inc.

#pragma once

namespace Sunset
{
	using TextCharacters = std::map<char, struct TextCharacter>;
	class Text
	{
	public:
		explicit Text(const std::string& text);
		virtual ~Text();
		void SetFont(const std::shared_ptr<TextCharacters>& textFont);
		void Render();

	private:

		std::string m_Text;
		std::shared_ptr<TextCharacters> m_TextFont;
	};
}
