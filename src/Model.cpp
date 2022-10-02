#include "Model.h"
Model::Model(Shader* shader, const std::string location)
{
    m_material = new Material(shader);
    m_vao = nullptr;
    m_ib = nullptr;
    CreateMesh(location);
}

void Model::CreateMesh(const std::string location)
{
    if(m_vao != nullptr)
        delete(m_vao);
    if(m_ib != nullptr)
        delete(m_ib);

    std::ifstream inputStream;
    std::string stringLine;
    m_FilePath = location;
    m_FilePath.resize(100);

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
                if (ss.good())
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
				// if object has four vertices, split it into two triangles
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
        }
    }
    inputStream.close();

    std::vector<Vertex> vertices;

    vertices.resize(iPositions.size(), Vertex());

    for (size_t i = 0; i < vertices.size(); i++)
    {
        vertices[i].position = vPositions[iPositions[i] - 1];
        vertices[i].texcoord = vTexCoords[iTexCoords[i] - 1];
        vertices[i].normal = vNormals[iNormals[i] - 1];
    }

    VertexBufferLayout layout;

    layout.Push<float>(3);
    layout.Push<float>(2);
    layout.Push<float>(3);

    GLCall(m_vao = new VertexArray(vertices.data(), sizeof(vertices[0]) * vertices.size(), layout));
}

