#include "Model.h"

Model::Model(const std::string&& filePath, Material* material)
    :m_FilePath(filePath), m_material(material)
{
    std::ifstream inputStream;
    std::string stringLine;

    inputStream.open(m_FilePath);

    if (!inputStream.is_open())
    {
        std::cout << "OBJ FILE NOT FOUND!\n";
    }

    std::vector<glm::vec3> vPositions;
    std::vector<glm::vec2> vTexCoords;
    std::vector<glm::vec3> vNormals;

    std::vector<uint32_t> iPositions;
    std::vector<uint32_t> iTexCoords;
    std::vector<uint32_t> iNormals;


    while (std::getline(inputStream, stringLine) && inputStream.good())
    {
        const std::string header(stringLine.substr(0, stringLine.find_first_of(' ')));
        const std::string content(stringLine.substr(header.length() + 1, stringLine.length() - header.length() - 1));

       // std::cout << "Header: \"" << header << "\", Content: \"" << content << "\"\n";

        if (header == "v")
        {
            std::stringstream ss(content);
            glm::vec3 pos(0.0f);

            for (int i = 0; i < 3; i++)
            {
                std::string tmp{ "" };
                ss >> tmp;

                pos[i] = std::stod(tmp);

            }

            vPositions.emplace_back(pos);
        }
        else if (header == "vt")
        {
            std::stringstream ss(content);
            glm::vec2 pos(0.0f);

            for (int i = 0; i < 2; i++)
            {
                std::string tmp{ "" };
                ss >> tmp;
                pos[i] = std::stod(tmp);
            }
            vTexCoords.emplace_back(pos);
        }
        else if (header == "vn")
        {
            std::stringstream ss(content);
            glm::vec3 pos(0.0f);

            for (int i = 0; i < 3; i++)
            {
                std::string tmp{ "" };
                ss >> tmp;
                pos[i] = std::stod(tmp);
            }
            vNormals.emplace_back(pos);
        }
        else if (header == "f")
        {
            std::istringstream ss(content);

            unsigned int vertAmount;

            std::string verticesCount[4];
			
            std::istringstream ss2(content);
            
            for (auto& v : verticesCount)
            {
				if(ss.good())
                    ss >> v; 
            }

            if (verticesCount[3] != "")
            {
                std::string vertices[4];

                for (auto& v : vertices)
                {
                    
                    static int h = 0;
                    h++;
                    ss2 >> v;
					
                    //std::cout << v << " checking vertices" << std::endl;
                }

                unsigned int quadPos[4];
                unsigned int quadTextCoord[4];
                unsigned int quadNormals[4];
				

                for (int i = 0; i < 4; i++)
                {
                    std::stringstream amon(vertices[i]);
                    std::string str{ "" };
                    str.reserve(8);

                        std::getline(amon, str, '/');
						
                        quadPos[i] = std::stoi(str);


                        std::getline(amon, str, '/');
                        quadTextCoord[i] = std::stoi(str);



                        std::getline(amon, str, '/');
                        quadNormals[i] = std::stoi(str);

                        
                }
                //std::cout << "[DONE] inserting to quads\n";
				
                iPositions.emplace_back(quadPos[0]);
                iTexCoords.emplace_back(quadTextCoord[0]);
                iNormals.emplace_back(quadNormals[0]);
				
                iPositions.emplace_back(quadPos[1]);
                iTexCoords.emplace_back(quadTextCoord[1]);
                iNormals.emplace_back(quadNormals[1]);
				
                iPositions.emplace_back(quadPos[2]);
                iTexCoords.emplace_back(quadTextCoord[2]);
                iNormals.emplace_back(quadNormals[2]);

				iPositions.emplace_back(quadPos[0]);
				iTexCoords.emplace_back(quadTextCoord[0]);
				iNormals.emplace_back(quadNormals[0]);

				iPositions.emplace_back(quadPos[2]);
				iTexCoords.emplace_back(quadTextCoord[2]);
				iNormals.emplace_back(quadNormals[2]);

				iPositions.emplace_back(quadPos[3]);
				iTexCoords.emplace_back(quadTextCoord[3]);
				iNormals.emplace_back(quadNormals[3]);
                
                //std::cout << "[DONE] ipushes\n";
			}			
            else if (verticesCount[3] == "")
            {
                std::string vertices[3];

                for (auto& v : vertices)
                    ss2 >> v;

                for (int i = 0; i < 3; i++)
                {
                    std::stringstream ss(vertices[i]);
                    std::string str{ "" };
                    str.reserve(8);

                    std::getline(ss, str, '/');
                    iPositions.emplace_back(std::stoi(str));

                    std::getline(ss, str, '/');
                    iTexCoords.emplace_back(std::stoi(str));

                    std::getline(ss, str, '/');
                    iNormals.emplace_back(std::stoi(str));
                }
            }
            //std::cout << "[DONE] if statement \n\n" << std::endl;
        }   
    }
    inputStream.close();

    std::vector<Vertex> vertices;
    //std::cout << "before resize\n";
    vertices.resize(iPositions.size(), Vertex());
    //std::cout << "after resize\n";

    for (size_t i = 0; i < vertices.size(); i++)
    {
        vertices[i].position = vPositions[iPositions[i] - 1];
        vertices[i].texcoord = vTexCoords[iTexCoords[i] - 1];
        vertices[i].normal = vNormals[iNormals[i] - 1];
    }
    //std::cout << "end of the loop\n";

    VertexBufferLayout layout;

    layout.Push<float>(3);
    layout.Push<float>(2);
    layout.Push<float>(3);

    GLCall(m_vao = new VertexArray(vertices.data(), sizeof(vertices[0]) * vertices.size(), layout));

    glActiveTexture(GL_TEXTURE0);
    m_material->albedoTexture->Bind(0);
    m_material->m_shader->SetInt("u_Texture", 0 );

    glActiveTexture(GL_TEXTURE1);
    if (m_material->specularTexture != nullptr)
    {
        m_material->specularTexture->Bind(1);
        m_material->m_shader->SetInt("u_specularTexture", 1);
    }
        


}

